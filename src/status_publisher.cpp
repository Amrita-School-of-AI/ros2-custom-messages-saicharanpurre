#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
// Included the generated message header
#include "ros2_custom_msgs/msg/robot_status.hpp"

using namespace std::chrono_literals;

/*
 * Create a Class named 'StatusPublisher' that inherits from rclcpp::Node.
 * Requirements:
 * 1. The constructor should name the node "status_publisher".
 * 2. Create a publisher to topic "/robot_status" with RobotStatus message type.
 * 3. Create a timer that triggers every 1000ms.
 * 4. In timer callback:
 * - Set robot_name to "Explorer1"
 * - Decrease battery_level from 100.0 by 0.5 each tick
 * - Set is_active to true
 * - Increment mission_count from 0
 * - Publish and log the message
 */

class StatusPublisher : public rclcpp::Node
{
public:
    StatusPublisher()
        : Node("status_publisher"), battery_level_(100.0), mission_count_(0)
    {
        // Create publisher here
        publisher_ = this->create_publisher<ros2_custom_msgs::msg::RobotStatus>("/robot_status", 10);

        // Create timer here
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&StatusPublisher::timer_callback, this));
    }

private:
    // Define timer_callback function here
    void timer_callback()
    {
        auto message = ros2_custom_msgs::msg::RobotStatus();
        message.robot_name = "Explorer1";
        message.battery_level = battery_level_;
        message.is_active = true;
        message.mission_count = mission_count_;

        RCLCPP_INFO(this->get_logger(), "Publishing: robot=%s, battery=%.1f, active=%s, missions=%d",
                    message.robot_name.c_str(), 
                    message.battery_level, 
                    message.is_active ? "true" : "false", 
                    message.mission_count);

        publisher_->publish(message);

        battery_level_ -= 0.5;
        mission_count_++;
    }

    rclcpp::TimerBase::SharedPtr timer_;
    // Declare publisher
    rclcpp::Publisher<ros2_custom_msgs::msg::RobotStatus>::SharedPtr publisher_;
    double battery_level_;
    int32_t mission_count_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatusPublisher>());
    rclcpp::shutdown();
    return 0;
}