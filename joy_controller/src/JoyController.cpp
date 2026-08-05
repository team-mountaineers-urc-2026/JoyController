/**
 * @file JoyController.cpp
 * @author William Streck
 * @brief JoyController main execution path.
 * @version 0.1
 * @date 2025-02-25
 * 
 */

#include <JC_include.hpp>

shared_ptr<JoyController> joy_controller;
double scaled_deadzone_;
double unscaled_deadzone_;
double scale_;
// TODO we need controller state

JoyController::JoyController() : Node("joy_controller") {
	
	this->declare_parameter(JOY_CONTROLLER_TOPIC_PARAM, JOY_CONTROLLER_TOPIC_DEFAULT);
	this->declare_parameter(JOY_POLL_PARAM, JOY_POLL_DEFAULT);
	this->declare_parameter(JOY_PUBLISH_PARAM, JOY_PUBLISH_DEFAULT);
	this->declare_parameter(JOY_PARAM, JOY_DEFAULTS);
	this->declare_parameter(JOY_DEADZONE_PARAM, JOY_DEADZONE_DEFAULT);

	auto joy_pub_topic = this->get_parameter(JOY_CONTROLLER_TOPIC_PARAM).as_string();

	auto joy_poll_rate = this->get_parameter(JOY_POLL_PARAM).as_int();
	auto poll_timer_delay_ms = 1000/joy_poll_rate;

	auto joy_publish_rate = this->get_parameter(JOY_PUBLISH_PARAM).as_int();
	auto publish_timer_delay_ms = 1000/joy_publish_rate;

	dev_ = this->get_parameter(JOY_PARAM).as_string();

	scaled_deadzone_ = this->get_parameter(JOY_DEADZONE_PARAM).as_double();
	unscaled_deadzone_ = 32767.0 * scaled_deadzone_;

  	scale_ = static_cast<float>(-1.0 / (1.0 - scaled_deadzone_) / 32767.0);

    // TODO figure out actual publishers
    publisher_ = this->create_publisher<sensor_msgs::msg::Joy>
        (joy_pub_topic, 10);

	poll_timer_ = this->create_wall_timer
		(chrono::milliseconds(poll_timer_delay_ms), bind(&JoyController::poll_controller_callback, this));

	publish_timer_ = this->create_wall_timer
		(chrono::milliseconds(publish_timer_delay_ms), bind(&JoyController::publish_controller_callback, this));
}

int JoyController::open_controller(string dev) {
	auto fd = open(dev.c_str(), O_RDONLY | O_NONBLOCK);
	// TODO remove (we don't actually care here)
	if (fd == -1) {
		// This is fine - we'll try again soon
	}

	return fd;
}

void JoyController::safe_open(bool open_dev) {
	
	if (open_dev) {
		if (dev_connected_) {
			close(dev_fd_);
		}
		dev_fd_ = open_controller(dev_);
		if (dev_fd_ != -1) {
			dev_connected_ = true;
		}
	}
}

void JoyController::poll_controller_callback() {
	// Make sure controllers are connected
	safe_open(!dev_connected_);

	// TODO need to find a way to see when they get pulled

	if (dev_connected_) {
		struct js_event e;
		ssize_t res;
		while ((res = read(dev_fd_, &e, sizeof(e))) > 0) {
			if (e.type == JS_EVENT_BUTTON) {
				set_button_state(e.number, e.value);
				// RCLCPP_INFO(this->get_logger(), "Button event: %d %d %d", e.time, e.value, e.number);
			} else if (e.type == JS_EVENT_AXIS) {
				// RCLCPP_INFO(this->get_logger(), "Axis event: %d %d %d", e.time, e.value, e.number);
				set_axis_state(e.number, e.value);
			} else {
				// Can figure out whether to break or not
				// RCLCPP_INFO(this->get_logger(), "Special event: %d %d %d %d", e.time, e.value, e.type, e.number);
			}
		}

		if (res == -1) {
			// We specifically need to find the no such device error - resource temporarily unavailable is fine and means no more events for now
			if (errno == ENODEV) {
				dev_connected_ = false;
			}
		}
	}
}

void JoyController::publish_controller_callback() {
	
	if (!dev_connected_) {
		return;
	}
	
	auto state = dev_state;
	auto msg = sensor_msgs::msg::Joy();
	msg.header.stamp = this->now();
	msg.header.frame_id = "joy";

	msg.buttons.clear();
	for (int i = 0; i < 16; i++) {
		msg.buttons.push_back((state.buttons >> i) & 1);
	}

	msg.axes.clear();
	for (auto i : state.axes) {
		// auto f = (float)i / AXIS_MAX;
		// msg.axes.push_back(f);
		msg.axes.push_back(i);
	}

	publisher_->publish(msg);
}

int main(int argc, char* argv[]) {
  	rclcpp::init(argc, argv);
	joy_controller = make_shared<JoyController>();
  	rclcpp::spin(joy_controller);
  	rclcpp::shutdown();

  	return 0;
}

void set_button_state(uint8_t button, int16_t value) {
	// Valid ID, valid button state - buttons change between controller settings
	if (value != BUTTON_EVENT_PRESSED && value != BUTTON_EVENT_RELEASED) {
		return;
	}

	dev_state.buttons &= ~(1 << button); // Unset regardless
	dev_state.buttons |= value << button; // Set if pressed
}

void set_axis_state(uint8_t axis, int16_t value) {
	// Valid ID, valid value (literally only one value doesn't work)
	if (value < AXIS_MIN) {
		return;
	}

	// Deadzone Code
	double double_val = static_cast<double>(value);
		if (double_val > unscaled_deadzone_) {
		double_val -= unscaled_deadzone_;
	} else if (double_val < -unscaled_deadzone_) {
		double_val += unscaled_deadzone_;
	} else {
		double_val = 0.0;
	}

	dev_state.axes[axis] = double_val * scale_;
}
