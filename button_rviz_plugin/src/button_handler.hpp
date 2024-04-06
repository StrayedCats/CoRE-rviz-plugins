// Copyright 2024 StrayedCats.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/empty.hpp>

namespace button_rviz_plugin
{
class ButtonHandler
{
public:
  ButtonHandler(void) {}

  void setRosNodePtr(const rclcpp::Node::SharedPtr node_ptr)
  {
    node_ptr_ = node_ptr;
  }

  bool initializePublisher(const std::string topic_name)
  {
    if (topic_name == "") {
      return false;
    }
    button_publisher_ =
      node_ptr_->create_publisher<std_msgs::msg::Empty>(topic_name, 10);
    return true;
  }

  void finalizePublisher(void)
  {
    button_publisher_.reset();
  }

  void publishButton(bool button)
  {
    if (button)
    {
      std_msgs::msg::Empty msg;
      button_publisher_->publish(msg);
    }
  }

  std::vector<std::string> getEmptyTopicList(void) const
  {
    return getTopicList("std_msgs/msg/Empty");
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
  rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr button_publisher_;
};

} // namespace button_rviz_plugin
