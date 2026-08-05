# JoyController

JoyController is a custom joystick control library that removes our dependencies on ROS2 packages. This node instead references the device specifically by their UDEV device ID.

Note that each node listens to both controllers. If you want to launch two different nodes on different laptops, give them different names in their launch files.

## License

JoyController is licensed under the MIT License.

## Table of Contents

- [Inputs and Outputs](#inputs-and-outputs)
    - [ROS2 Inputs](#ros2-inputs)
    - [ROS2 Outputs](#ros2-outputs)
    - [Streaming Output](#streaming-output)

## Inputs and Outputs

JoyController has ample inputs for controllers. Please note other utilities require configuration files for controllers - JoyController does not contain these configurations.

### Parameters
| Parameter | Type | Default | Description |
| --- | --- | --- | --- |
| `joy_controller_topic` | String | "joy" | Publisher topic for the JoyController |
| `JC_poll_rate` | Int64 | 250 | Frequency to poll the controller hardware |
| `JC_publish_rate` | Int64 | 100 | Frequency to publish ROS2 Joy messages |
| `device` | String | "/dev/input/js0" | The absolute path to the joy controller or its udev symlink |
| 'deadzone' | Float32 | 0.05 | The threshold under which values will be set to zero |

### Subscriptions
This node has no subscriptions. It reads from hardware.

### Publishers
Note topics may change based on launch file parameters. Defaults are listed.
| Topic | Type | Description |
| --- | --- | --- |
| `joy` | Joy | Joy output message; header frame ID contains controller ID |
