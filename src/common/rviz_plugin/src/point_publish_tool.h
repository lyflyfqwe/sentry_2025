
#pragma once
#include <rviz_common/tool.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <rclcpp/rclcpp.hpp>

class PointPublishTool : public rviz_common::Tool
{
  Q_OBJECT
public:
  PointPublishTool();
  virtual ~PointPublishTool() = default;
  virtual void activate() override;
  virtual void deactivate() override;
  void onInitialize() override;
  int processMouseEvent(rviz_common::ViewportMouseEvent& event) override;

private:
  bool is_drawing;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_1;
};