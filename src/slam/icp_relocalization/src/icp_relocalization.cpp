#include "icp_relocalization/icp_relocalization.h"

namespace icp_relocalization{

void IcpRelocalization::bodyCloudCallBack(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
    last_scan_time_ = msg->header.stamp;
    pcl::fromROSMsg(*msg, *cloud_body_scan_);
    source_ = small_gicp::voxelgrid_sampling_omp<pcl::PointCloud<pcl::PointXYZ>, pcl::PointCloud<pcl::PointCovariance>>(
        *cloud_body_scan_, size_registered_leaf_
    );
    small_gicp::estimate_covariances_omp(*source_, num_neighbour_, num_threads_);
    source_tree_ = std::make_shared<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>>(
        source_, small_gicp::KdTreeBuilderOMP(num_threads_)
    );
}

void IcpRelocalization::registrationCallBack(){
    if(!source_ || !source_tree_) return;
    debug_tools::Timer time_begin;

    register_->reduction.num_threads = num_threads_;
    register_->rejector.max_dist_sq = max_dist_sq_;

    auto result = register_->align(*target_, *source_, *target_tree_, result_last_);
    if(!result.converged){
        debug_tools::Debug().print("icp converge failed...");
        return;
    }

    result_now_ = result.T_target_source;
    result_last_ = result.T_target_source;
    debug_tools::Timer time_end(time_begin); time_end.log("ICP", "ms");
}

void IcpRelocalization::pubTransform(){
    if(result_now_.matrix().isZero()) return;
    geometry_msgs::msg::TransformStamped msg_tf;
    msg_tf.header.stamp = this->now() + rclcpp::Duration::from_seconds(0.5);
    msg_tf.header.frame_id = frame_map_;
    msg_tf.child_frame_id = frame_camera_init_;

    Eigen::Vector3d trans = result_now_.translation();
    Eigen::Quaternion<double> rot(result_now_.rotation());

    msg_tf.transform.translation.x = trans.x();
    msg_tf.transform.translation.y = trans.y();
    msg_tf.transform.translation.z = trans.z();
    msg_tf.transform.rotation.x = rot.x();
    msg_tf.transform.rotation.y = rot.y();
    msg_tf.transform.rotation.z = rot.z();
    msg_tf.transform.rotation.w = rot.w();

    // debug_tools::Debug().print("trans", trans.x(), trans.y(), trans.z());
    // debug_tools::Debug().print("rot", rot.x(), rot.y(), rot.z(), rot.w());

    tf_broadcaster_->sendTransform(msg_tf);
}

IcpRelocalization::IcpRelocalization(std::string node_name, const rclcpp::NodeOptions option): Node(node_name, option){
    frame_map_ = declare_parameter<std::string>("frame_map", "map");
    frame_camera_init_ = declare_parameter<std::string>("frame_camera_init", "camera_init");
    num_threads_ = declare_parameter<int>("num_threads", 4);
    num_neighbour_ = declare_parameter<int>("num_neighbour", 20);
    size_registered_leaf_ = declare_parameter<float>("size_registered_leaf", 0.25);
    max_dist_sq_ = declare_parameter<float>("max_dist_sq", 5.0);

    result_last_.linear() = Eigen::Quaternion<double>(1, 0, 0, 0).toRotationMatrix();
    result_now_.linear() = Eigen::Quaternion<double>(1, 0, 0, 0).toRotationMatrix();

    std::string path_global_pcd_ = declare_parameter<std::string>("path_global_pcd", "/home/ld/Documents/point_cloud/copy/labal_inside.pcd");
    cloud_global_map_ = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    if(pcl::io::loadPCDFile<pcl::PointXYZ>(path_global_pcd_, *cloud_global_map_) == -1){
        debug_tools::Debug().print("read pcd failed...");
        return;
    }
    cloud_body_scan_ = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();

    register_ = std::make_shared<small_gicp::Registration<small_gicp::GICPFactor, small_gicp::ParallelReductionOMP>>();

    target_ = small_gicp::voxelgrid_sampling_omp<pcl::PointCloud<pcl::PointXYZ>, pcl::PointCloud<pcl::PointCovariance>>(
        *cloud_global_map_, size_registered_leaf_
    );
    target_tree_ = std::make_shared<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>>(
        target_, small_gicp::KdTreeBuilderOMP(num_threads_)
    );

    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);

    std::string topic_global_cloud = declare_parameter<std::string>("topic_global_cloud", "/global_cloud");
    pub_global_cloud_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(topic_global_cloud, 10);

    std::string topic_cloud_body = declare_parameter<std::string>("topic_cloud_body", "/cloud_registered_body");
    sub_cloud_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(topic_cloud_body, 10, 
        std::bind(&IcpRelocalization::bodyCloudCallBack, this, std::placeholders::_1)    
    );

    timer_icp_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&IcpRelocalization::registrationCallBack, this));

    timer_send_tf_ = this->create_wall_timer(std::chrono::milliseconds(50), std::bind(&IcpRelocalization::pubTransform, this));

    timer_visual_ = this->create_wall_timer(std::chrono::milliseconds(1000), [this](){
        sensor_msgs::msg::PointCloud2 msg_cloud;
        pcl::toROSMsg(*cloud_global_map_, msg_cloud);
        msg_cloud.header.stamp = this->now();
        msg_cloud.header.frame_id = frame_map_;

        pub_global_cloud_->publish(msg_cloud);
    });
}

}

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto icp_node = std::make_shared<icp_relocalization::IcpRelocalization>("icp_node", rclcpp::NodeOptions());
    rclcpp::spin(icp_node);
    rclcpp::shutdown();
    return 0;
}