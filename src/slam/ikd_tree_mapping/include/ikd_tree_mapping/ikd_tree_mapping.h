#ifndef IKD_TREE_MAPPING_H
#define IKD_TREE_MAPPING_H

#include <execution>

#include <Eigen/Eigen>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/pcd_io.h>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <tf2_ros/transform_broadcaster.h>

#include "rm_interface/srv/save_pcd.hpp"
#include "debug_tools/debug_tools.h"
#include "ikd_tree_mapping/ikd_Tree_impl.h"

template class ikdtreeNS::KD_TREE<pcl::PointXYZ>;
namespace ikd_tree_mapping{

class IkdTreeMapping: public rclcpp::Node{
private:
    struct CloudOdomGroup{
        typedef std::shared_ptr<message_filters::TimeSynchronizer<sensor_msgs::msg::PointCloud2, nav_msgs::msg::Odometry>> SynchronizerCloudOdom;
        SynchronizerCloudOdom syn_chronizer;
        std::shared_ptr<message_filters::Subscriber<sensor_msgs::msg::PointCloud2>> sub_cloud;
        std::shared_ptr<message_filters::Subscriber<nav_msgs::msg::Odometry>> sub_odom;
        std::string topic_cloud, topic_odom;
    }group_add_, group_del_;

    struct Config{
        float ikd_delete_param, ikd_balance_param, ikd_box_length;
        ikdtreeNS::BoxPointType range_visual, range_mapping;
        Eigen::Vector<float, 6> range_FOV; //0:v_min,1:v_max,2:h_min,3:h_max, 4:closed_range^2, 5:far_range^2
        Eigen::Vector3f ext_p; Eigen::Matrix3f ext_m;
        std::string frame_lidar, frame_odom;
        bool is_pub_FOV;
    }cfg_;

    struct VisualGroup{
        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_map_visual;
        rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr pub_FOV;
        Eigen::Vector3f odom_pos_for_visual;
        std::string topic_map, topic_FOV;
        rclcpp::TimerBase::SharedPtr timer_visual;
        float mil_timer, mil_time_thres;

        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    }group_visual;

    rclcpp::Service<rm_interface::srv::SavePCD>::SharedPtr service_save_pcd_;
    ikdtreeNS::KD_TREE<pcl::PointXYZ>::Ptr kd_tree_;

    inline bool checkFOV(const Eigen::Vector<float, 6>& range_FOV, const pcl::PointXYZ& p); 

    void callBackCloudOdom(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg_cloud, const nav_msgs::msg::Odometry::ConstSharedPtr& msg_odom);

    Eigen::Quaternionf eulerToQuaternion(float roll, float pitch, float yaw);

    void pubMap(rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr& pub, 
                std::vector<pcl::PointXYZ, Eigen::aligned_allocator<pcl::PointXYZ>>& points);

    void pubFOV(Eigen::Vector<float, 6>& fov, rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr& pub);

    ikdtreeNS::BoxPointType getSmallBox(ikdtreeNS::BoxPointType& box_in, float rate);

    inline void pubTransform(const Eigen::Vector3f& tran);

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    IkdTreeMapping();
};
};

#endif