/**
 * @file JoyController_Node.hpp
 * @author William Streck
 * @brief Defines the JoyController Node in case you want to move it anywhere else.
 * @version 0.1
 * @date 2025-02-25
 * 
 */

#ifndef JOY_CONTROLLER_NODE_HPP
#define JOY_CONTROLLER_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int32.hpp>
#include <sensor_msgs/msg/joy.hpp>

using namespace std;

#define JOY_CONTROLLER_TOPIC_DEFAULT "joy"
#define JOY_CONTROLLER_TOPIC_PARAM "joy_controller_topic"
// #define JOY_POLL_DEFAULT 250
#define JOY_POLL_DEFAULT 250
#define JOY_POLL_PARAM "JC_poll_rate"
#define JOY_PUBLISH_DEFAULT 100
#define JOY_PUBLISH_PARAM "JC_publish_rate"
// TODO set these up for actual defaults, even though we have launch files
#define JOY_DEVICE_DEFAULT "/dev/input/js0" // Only for debug
#define JOY_PARAM "device"
#define JOY_DEFAULTS std::string(JOY_DEVICE_DEFAULT)
#define JOY_DEADZONE_DEFAULT 0.05
#define JOY_DEADZONE_PARAM "deadzone"

class JoyController : public rclcpp::Node {
    public:
        JoyController();

    private:
        rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr publisher_; ///< CameraManager debug response publisher.

        rclcpp::TimerBase::SharedPtr poll_timer_; ///< Timer for polling the controller.
        rclcpp::TimerBase::SharedPtr publish_timer_; ///< Timer for publishing the controller state.

        string dev_;
        int dev_fd_;
        bool dev_connected_ = false;

        /**
         * @brief Polls the controller for all events that are present.
         * 
         */
        void poll_controller_callback();

        /**
         * @brief Publishes the controller state.
         * 
         */
        void publish_controller_callback();

        /**
         * @brief Opens a controller from the dev file for reading.
         * 
         * @param dev Device file to open.
         * @return int File descriptor for the controller.
         */
        int open_controller(string dev);

        /**
         * @brief Tries to open controllers as specified.
         * 
         * @param arm Should we try to open the arm controller?
         * @param drivebase Should we try to open the drivebase controller?
         */
        void safe_open(bool open_dev);
};

extern shared_ptr<JoyController> joy_controller;

#endif
