#ifndef RM_DECISION_VIS_H
#define RM_DECISION_VIS_H

#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <Eigen/Eigen>
#include "rm_decision/polygon_region.h"

namespace rm_decision{
class Visual{
public:
    void pubCubeVisual(rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr& pub,  
            rclcpp::Node* node,  
            const std::vector<Eigen::Vector3d>& centers,  
            std::initializer_list<float> cube_size,
            std::initializer_list<float> color_rgb){
        if (pub->get_subscription_count() < 1) return;  
        if (color_rgb.size() != 3) return;  

        auto c = color_rgb.begin();  
        float rgb[3]{*(c), *(c+1), *(c+2)};  
                    
        visualization_msgs::msg::MarkerArray marker_array;  
        int id{0}; // ID for each cube  

        auto s = cube_size.begin();

        visualization_msgs::msg::Marker marker;  
        marker.header.frame_id = "map";  
        marker.header.stamp = node->now();  
        marker.ns = "cubes";
        marker.type = visualization_msgs::msg::Marker::CUBE; 
        marker.action = visualization_msgs::msg::Marker::DELETEALL;
        marker_array.markers.push_back(marker);  
                    
        for (const auto& center : centers){
             
            marker.id = ++id;   
            marker.action = visualization_msgs::msg::Marker::ADD;  

            // 设置颜色  
            marker.color.r = rgb[0];  
            marker.color.g = rgb[1];  
            marker.color.b = rgb[2];  
            marker.color.a = 1.0;

            // 设置尺寸  
            marker.scale.x = *s++;  
            marker.scale.y = *s++;  
            marker.scale.z = *s;  

            // 设置位置  
            marker.pose.position.x = center.x();  
            marker.pose.position.y = center.y();  
            marker.pose.position.z = center.z();  
            marker.pose.orientation.w = 1.0; 

            marker_array.markers.push_back(marker);  
        }  

        pub->publish(marker_array);  
    }  

    /**
     * @brief 从ros参数中加载构成多边形区域的点坐标
     * @param param_name 对应参数名
     * @param region 多边形区域
     * @param node ros节点指针
     * @param default_param 默认参数 exp: {1, 2, 3}
     * @return result
     */
    template<typename T>
    inline bool loadParamToRegion(const std::string param_name, PolygonRegion& region, 
            rclcpp::Node* node, const std::vector<T, std::allocator<T>>& default_param){
        auto points = node->declare_parameter<std::vector<T, std::allocator<T>>>(param_name, default_param);
        auto p_size = static_cast<int>(points.size());
        if(p_size <= 0 || p_size % 3 != 0){
            debug_tools::Debug().print("vecsToRegion failed:", param_name);
            return false;
        }
        for(int i{0}; i < p_size; i+= 3) region.push_back(Eigen::Vector3d(points.at(i), points.at(i+1), points.at(i+2)));
            
        return true;
    }
}; //class Visual
} //namespace rm_decision

#endif