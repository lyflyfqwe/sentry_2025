#ifndef ICP_RELOCALIZATION_SYNC_H
#define ICP_RELOCALIZATION_SYNC_H

#include <memory>
#include <execution>

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
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
#include <std_msgs/msg/bool.hpp>

#include "debug_tools/debug_tools.h"

namespace icp_relocalization{
class IcpRelocalization: public rclcpp::Node{
private:
    rclcpp::SubscriptionOptionsWithAllocator<std::allocator<void>> options_;
    rclcpp::CallbackGroup::SharedPtr group_;

    std::shared_ptr<message_filters::TimeSynchronizer<sensor_msgs::msg::PointCloud2, nav_msgs::msg::Odometry>> syn_chronizer_;
    std::shared_ptr<message_filters::Subscriber<sensor_msgs::msg::PointCloud2>> sub_cloud_;
    std::shared_ptr<message_filters::Subscriber<nav_msgs::msg::Odometry>> sub_odom_;
    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr sub_init_pose_;

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_global_map_; 
    pcl::PointCloud<pcl::PointCovariance>::Ptr source_, target_;
    std::shared_ptr<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>> source_tree_, target_tree_;

    std::string frame_map_, frame_odom_;
    int num_threads_, num_neighbour_;
    float size_registered_leaf_, max_dist_sq_;
    Eigen::Isometry3d result_now_, result_last_;

    std::shared_ptr<small_gicp::Registration<small_gicp::GICPFactor, small_gicp::ParallelReductionOMP>> register_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_odom_;
    std::thread td_icp_, td_tf_, td_visual_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_global_cloud_;

    float max_cloud_p_dist_sq_; //超过这个距离的点云将被忽略
    std::vector<double> FOV_;

    bool stop_flag_{false}; // 控制icp线程运行的标志
    std::mutex mutex_sensor_, mutex_switch_; 
    std::condition_variable cv_; 
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_switch_;

    struct TF{
        Eigen::Vector3d tran = Eigen::Vector3d::Zero();
        double yaw{0};
    }tf_;
    

    inline double getYaw(const Eigen::Matrix3d& mat){
        return std::atan2(mat(1, 0), mat(0, 0));
    }

    inline Eigen::Quaterniond yawToQuaternion(double yaw_rad){
        return Eigen::Quaterniond(Eigen::AngleAxisd(yaw_rad, Eigen::Vector3d::UnitZ()));
    }
    


    void callBackCloudOdom(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg_cloud, const nav_msgs::msg::Odometry::ConstSharedPtr& msg_odom);

    void callBackInitPose(const geometry_msgs::msg::PoseWithCovarianceStamped::ConstSharedPtr& msg);

    inline void tryRegistration();
    void callBackRegistration(int time_itv);

    inline void tryPubTransform();
    void pubTransform(int time_itv);

    void callBackSwitch(const std_msgs::msg::Bool::ConstSharedPtr& msg);

public:
    IcpRelocalization();

    ~IcpRelocalization();
}; //class IcpRelocalization
} //namespace icp_relocalization

#endif