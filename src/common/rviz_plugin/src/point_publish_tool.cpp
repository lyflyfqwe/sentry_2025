#include "point_publish_tool.h"
#include <rviz_common/logging.hpp>

#include <rviz_common/display_context.hpp>
#include <rviz_common/viewport_mouse_event.hpp>
#include <rviz_common/render_panel.hpp>  // 提供 RenderWindow 定义
#include <rviz_common/view_manager.hpp>


#include <OgreRay.h>           // Ogre 射线计算
#include <OgrePlane.h>         // Ogre 平面方程
#include <OgreCamera.h>


PointPublishTool::PointPublishTool()
{
  setName("point publish");
  shortcut_key_ = 'c';
}

void PointPublishTool::onInitialize()
{
  node_ = std::make_shared<rclcpp::Node>("point_publish_tool");
  point_pub_1= node_->create_publisher<geometry_msgs::msg::PointStamped>(
    "rm_nav_goal", 10);
}

// 激活函数
void PointPublishTool::activate() {
    is_drawing = true;
    RVIZ_COMMON_LOG_INFO("Polygon_Simulation Activated!"); 
}
  
// 停用函数
void PointPublishTool::deactivate() {
    is_drawing = false;
    RVIZ_COMMON_LOG_INFO("Polygon_Simulation Deactivated!");
}

int PointPublishTool::processMouseEvent(rviz_common::ViewportMouseEvent& event){
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
        // msg.header.frame_id = context_->getFixedFrame().toStdString();
        msg.header.frame_id = "map";
        msg.point.x = pos.x;
        msg.point.y = pos.y;
        msg.point.z = pos.z;
        point_pub_1->publish(msg);
  
        RCLCPP_INFO(node_->get_logger(), "Published: (%.2f, %.2f, %.2f)", 
                    msg.point.x, msg.point.y, msg.point.z);
      }
      else
      {
        RCLCPP_WARN(node_->get_logger(), "Click position not on ground plane");
      }
    }
    return Render;
  }



#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(PointPublishTool, rviz_common::Tool)