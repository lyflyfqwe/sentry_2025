#include <iostream>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>  

#include "tilted_lidar/random_cloud.h"

namespace tilted_lidar{

class PublishBodyCloud: public rclcpp::Node{
private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_cloud_body_;
    sensor_msgs::msg::PointCloud2 msg_cloud_;

public:
    PublishBodyCloud(std::string node_name): Node(node_name){
        auto cloud_for_test = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
        generateDenseTetrahedronCloud(cloud_for_test, 0.01);
        for(auto& p : cloud_for_test->points){
            p.x += 2;
            p.y += 3;
            p.z += 1;
        }

        pcl::toROSMsg(*cloud_for_test, msg_cloud_);
        msg_cloud_.header.frame_id = "body";

        pub_cloud_body_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/cloud_body", 10);

        timer_ = this->create_wall_timer(std::chrono::milliseconds(50), [this](){
            msg_cloud_.header.stamp = this->now();
            pub_cloud_body_->publish(msg_cloud_);
        });
    }
};

}

// 生成正四面体的点云，模拟雷达直接发布的点云 或是经里程计处理后二次发布的,frame是imu坐标系的点云
// 循环发布frame为body的点云, 话题是/cloud_body, rviz上呈现白色

int main(int argc, char** argv){
    rclcpp::init(argc, argv);  
    rclcpp::spin(std::make_shared<tilted_lidar::PublishBodyCloud>("publish_body_cloud_node"));  
    rclcpp::shutdown();  
    return 0;  
}