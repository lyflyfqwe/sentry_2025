// click_coordinate_tool.cpp
#include "polygons_tool.h"
#include <rviz_common/display_context.hpp>
#include <rviz_common/viewport_mouse_event.hpp>
#include <rviz_common/render_panel.hpp>  // 提供 RenderWindow 定义

#include <rviz_common/view_manager.hpp>

#include <OgreRay.h>           // Ogre 射线计算
#include <OgrePlane.h>         // Ogre 平面方程
#include <rviz_common/view_controller.hpp>  // 视图控制器
#include <OgreCamera.h>
#include <rviz_common/logging.hpp>




#include <vector>

PolygonsTool::PolygonsTool()
{
  setName("polygons");
  shortcut_key_ = 'c';
}

void PolygonsTool::onInitialize()
{
  node_ = std::make_shared<rclcpp::Node>("polygons_tool");
  polygons_pub_ = node_->create_publisher<visualization_msgs::msg::MarkerArray>("polygons", 10);
  point_pub_1= node_->create_publisher<geometry_msgs::msg::PointStamped>(
    "/point_polygons", 10);
    point_pub_2= node_->create_publisher<geometry_msgs::msg::PointStamped>(
        "/point_polygons_2", 10);
    // clicked_point_sub_1 = node_->create_subscription<geometry_msgs::msg::PointStamped>(
    //     "/point_polygons", 10,
    //     [this](const geometry_msgs::msg::PointStamped::SharedPtr msg) {
    //       this->click_callback_1(*msg);
    //     });
    // clicked_point_sub_2 = node_->create_subscription<geometry_msgs::msg::PointStamped>(
    //       "/point_polygons_2", 10,
    //       [this](const geometry_msgs::msg::PointStamped::SharedPtr msg) {
    //         this->click_callback_2(*msg);
    //       });
}

// 激活函数
void PolygonsTool::activate() {
    is_drawing = true;
    RVIZ_COMMON_LOG_INFO("Polygon_Simulation Activated!");
}
  
// 停用函数
 void PolygonsTool::deactivate() {
    is_drawing = false;
    RVIZ_COMMON_LOG_INFO("Polygon_Simulation Deactivated!");
}

int PolygonsTool::processMouseEvent(rviz_common::ViewportMouseEvent& event){
    if (event.leftDown())
    {
      auto view_controller = context_->getViewManager()->getCurrent();
      if (!view_controller) {
        RCLCPP_ERROR(node_->get_logger(), "ViewController is null!");
        return Render;
      }
  
      // 归一化坐标（Y轴翻转）
      const float nx = static_cast<float>(event.x) / event.panel->width();
      const float ny = static_cast<float>(event.y) / event.panel->height();
  
      // 生成射线
      Ogre::Ray mouse_ray = view_controller->getCamera()->getCameraToViewportRay(nx, ny);
  
      // 定义地面平面（Z=0）
      Ogre::Plane ground_plane(Ogre::Vector3::UNIT_Z, 0.0f);
      auto intersection = mouse_ray.intersects(ground_plane);
  
      if (intersection.first)
      {
        Ogre::Vector3 pos = mouse_ray.getPoint(intersection.second);
  
        // 发布坐标
        geometry_msgs::msg::PointStamped msg;
        msg.header.stamp = node_->now();
        msg.header.frame_id = context_->getFixedFrame().toStdString();
        msg.point.x = pos.x;
        msg.point.y = pos.y;
        msg.point.z = pos.z;
        point_pub_1->publish(msg);
        click_callback_1(msg);
        RCLCPP_INFO(node_->get_logger(), "Published(polygons_tool): (%.2f, %.2f, %.2f)", 
                    msg.point.x, msg.point.y, msg.point.z);
      }
      else
      {
        RCLCPP_WARN(node_->get_logger(), "Click position not on ground plane");
      }
    }
    if(event.rightDown()){
        Ogre::Vector3 pos ={event.last_x,event.last_y,0};
        geometry_msgs::msg::PointStamped point;
          point.point.x = pos.x/100;
          point.point.y = pos.y/100;  
          point.point.z = 0.01; // 强制Z坐标为平面高度
          point_pub_2->publish(point);
          click_callback_2(point);
        RVIZ_COMMON_LOG_INFO("回退!");
      }
    return Render;
  }



void PolygonsTool::click_callback_2(const geometry_msgs::msg::PointStamped& msg){
    if(!points_.empty()){
      points_.pop_back();
      publish_all_polygons();
      
    }
    
  }
void PolygonsTool::click_callback_1(const geometry_msgs::msg::PointStamped& msg){
    RVIZ_COMMON_LOG_INFO("click_callback_1!");
      if (points_.empty()) {
        // 添加第一个点
        geometry_msgs::msg::Point32 pt32;
        pt32.x = static_cast<float>(msg.point.x);
        pt32.y = static_cast<float>(msg.point.y);
        pt32.z = static_cast<float>(msg.point.z);
        points_.push_back(pt32);
    } else {
        // 计算与第一个点的距离
        const auto& first_pt = points_.front();
        float dx = msg.point.x - first_pt.x;
        float dy = msg.point.y - first_pt.y;
        float distance = std::hypot(dx, dy);
  
        if (distance < 0.3) { // 闭合阈值0.1米
            // 闭合多边形：添加第一个点并保存
            points_.push_back(points_.front());
            save_current_polygon();
            points_.clear();
        } else {
            // 添加普通点
            geometry_msgs::msg::Point32 pt32;
            pt32.x = static_cast<float>(msg.point.x);
            pt32.y = static_cast<float>(msg.point.y);
            pt32.z = static_cast<float>(msg.point.z);
            points_.push_back(pt32);
        }
    }
    publish_all_polygons();
}




void PolygonsTool::publish_all_polygons() {
    visualization_msgs::msg::MarkerArray markers;
  
    // 添加已完成的多个边形
    for (const auto& marker : finished_polygons_) {
        markers.markers.push_back(marker);
    }
  
    // 添加当前绘制中的多边形（红色）
    if (!points_.empty()) {
        visualization_msgs::msg::Marker current_marker;
        current_marker.header.frame_id = "map";
        current_marker.header.stamp = node_->get_clock()->now();
        current_marker.ns = "current";
        current_marker.id = 0;
        current_marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
        current_marker.scale.x = 0.05;
        current_marker.color.a = 1.0;
        current_marker.color.r = 1.0; // 红色当前多边形
        current_marker.color.g = 0.0;
        current_marker.color.b = 0.0;
  
        for (const auto& pt : points_) {
            geometry_msgs::msg::Point p;
            p.x = pt.x;
            p.y = pt.y;
            p.z = pt.z;
            current_marker.points.push_back(p);
        }
        markers.markers.push_back(current_marker);
    }
  
    polygons_pub_->publish(markers);
}


void PolygonsTool::save_current_polygon() {
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "map";
    marker.header.stamp = node_->get_clock()->now();
    marker.ns = "polygons";
    marker.id = finished_polygons_.size(); // 唯一ID
    marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.scale.x = 0.05; // 线宽
    marker.color.a = 1.0;
    marker.color.r = 0.0; // 绿色已完成多边形
    marker.color.g = 1.0;
    marker.color.b = 0.0;

    // 转换Point32到Point
    for (const auto& pt : points_) {
        geometry_msgs::msg::Point p;
        p.x = pt.x;
        p.y = pt.y;
        p.z = pt.z;
        marker.points.push_back(p);
    }
    finished_polygons_.push_back(marker);
}


#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(PolygonsTool, rviz_common::Tool)