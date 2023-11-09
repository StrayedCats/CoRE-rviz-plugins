// MIT License

// Copyright (c) 2023 Project SRS

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>

namespace joy_rviz_plugin
{
class JoyHandler
{
public:
  JoyHandler(void) {}

  void setRosNodePtr(const rclcpp::Node::SharedPtr node_ptr)
  {
    node_ptr_ = node_ptr;
  }

  bool initializePublisher(const std::string topic_name)
  {
    if (topic_name == "") {
      return false;
    }
    joy_publisher_ =
      node_ptr_->create_publisher<sensor_msgs::msg::Joy>(topic_name, rclcpp::QoS(10));
    return true;
  }

  void finalizePublisher(void)
  {
    joy_publisher_.reset();
  }

  void publishJoy(std::vector<float> axes, std::vector<int> buttons)
  {
    sensor_msgs::msg::Joy msg{};
    msg.header.stamp = node_ptr_->now();
    msg.axes.resize(2);
    msg.axes = axes;
    msg.buttons = buttons;
    joy_publisher_->publish(msg);
  }

  std::vector<std::string> getTwistTopicList(void) const
  {
    return getTopicList("sensor_msgs/msg/Joy");
  }

private:
  std::vector<std::string> getTopicList(const std::string type_name) const
  {
    std::map<std::string,
      std::vector<std::string>> topic_map = node_ptr_->get_topic_names_and_types();

    std::vector<std::string> output;
    for (auto pair : topic_map) {
      for (auto s : pair.second) {
        if (s == type_name) {
          output.push_back(pair.first);
          break;
        }
      }
    }
    return output;
  }

  rclcpp::Node::SharedPtr node_ptr_;
  rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr joy_publisher_;
};

} // namespace joy_rviz_plugin
