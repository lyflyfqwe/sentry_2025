#ifndef ICP_RELOCALIZATION_H
#define ICP_RELOCALIZATION_H

#include <memory>

#include <Eigen/Dense>
#include <pcl/io/pcd_io.h>
#include <small_gicp/pcl/pcl_point.hpp>
#include <small_gicp/registration/reduction_omp.hpp>
#include <small_gicp/registration/registration.hpp>
#include <small_gicp/util/downsampling_omp.hpp>
#include <small_gicp/pcl/pcl_registration.hpp>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_ros/transform_broadcaster.h>

#include "debug_tools/debug_tools.h"

namespace icp_relocalization{

class IcpRelocalization: public rclcpp::Node{
private:
    std::string frame_map_, frame_camera_init_;
    int num_threads_, num_neighbour_;
    float size_registered_leaf_, max_dist_sq_;

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_global_map_, cloud_body_scan_; 
    pcl::PointCloud<pcl::PointCovariance>::Ptr source_, target_;
    std::shared_ptr<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>> source_tree_, target_tree_;

    rclcpp::Time last_scan_time_;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_cloud_;
    std::shared_ptr<small_gicp::Registration<small_gicp::GICPFactor, small_gicp::ParallelReductionOMP>> register_;
    Eigen::Isometry3d result_now_, result_last_;
    rclcpp::TimerBase::SharedPtr timer_icp_, timer_send_tf_, timer_visual_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_global_cloud_;

    void bodyCloudCallBack(const sensor_msgs::msg::PointCloud2::SharedPtr msg);

    void registrationCallBack();

    void pubTransform();

public:
    IcpRelocalization(std::string node_name, const rclcpp::NodeOptions option);
};
}

#endif