// click_coordinate_tool.hpp
#pragma once
#include <rviz_common/tool.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include "visualization_msgs/msg/marker_array.hpp"
#include "geometry_msgs/msg/point32.hpp" // 新增头文件


class PolygonsTool : public rviz_common::Tool
{
  Q_OBJECT
public:
  PolygonsTool();
  virtual ~PolygonsTool() = default;
  virtual void activate() override;
  virtual void deactivate() override;
  void onInitialize() override;
  void click_callback_2(const geometry_msgs::msg::PointStamped& msg);
  void click_callback_1(const geometry_msgs::msg::PointStamped& msg);
  void click_callback_2();
  void click_callback_1();
  void publish_all_polygons();
  void save_current_polygon(); 
  int processMouseEvent(rviz_common::ViewportMouseEvent& event) override;

private:
  bool is_drawing;
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_1;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_2;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr polygons_pub_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_1;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_point_sub_2;
  std::vector<geometry_msgs::msg::Point32> points_; // 使用Point32容器
  std::vector<visualization_msgs::msg::Marker> finished_polygons_;
};