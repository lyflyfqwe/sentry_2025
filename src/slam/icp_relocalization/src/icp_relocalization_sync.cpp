#include "icp_relocalization/icp_relocalization_sync.h"

namespace icp_relocalization{
void IcpRelocalization::callBackCloudOdom(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg_cloud, const nav_msgs::msg::Odometry::ConstSharedPtr& msg_odom){
    if(stop_flag_) return;
    auto tran = Eigen::Vector3f(msg_odom->pose.pose.position.x, msg_odom->pose.pose.position.y, msg_odom->pose.pose.position.z);
    auto qua = Eigen::Quaternionf(
        msg_odom->pose.pose.orientation.w, msg_odom->pose.pose.orientation.x, msg_odom->pose.pose.orientation.y, msg_odom->pose.pose.orientation.z).toRotationMatrix();
    auto cloud_in = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    pcl::fromROSMsg(*msg_cloud, *cloud_in);
    if(cloud_in->points.size() <= 10) return;
    
    std::for_each(std::execution::par, cloud_in->points.begin(), cloud_in->points.end(), [&](pcl::PointXYZ& p){
        Eigen::Vector3d point_origin(p.x, p.y, p.z);
        // 计算水平角和俯仰角（弧度）
        double theta = std::atan2(point_origin.y(), point_origin.x());
        double phi = std::atan2(point_origin.z(), point_origin.head<2>().norm());

        if(p.x*p.x + p.y+p.y + p.z*p.z >= max_cloud_p_dist_sq_
            || phi < FOV_[0] || phi > FOV_[1] || theta < FOV_[2] || theta > FOV_[3]) p.x = p.y = p.z = NAN;
        else{
            Eigen::Vector3f p_origin(p.x, p.y, p.z);
            Eigen::Vector3f p_world = qua * p_origin + tran;
            p.x = p_world.x();
            p.y = p_world.y();
            p.z = p_world.z();
        }
    });
    // 删除标记为NaN的点
    cloud_in->points.erase(
        std::remove_if(cloud_in->points.begin(), cloud_in->points.end(),
            [](const pcl::PointXYZ& p) {
                return std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z);
            }),
        cloud_in->points.end()
    );

    std::unique_lock<std::mutex> lock(mutex_sensor_);
    source_ = small_gicp::voxelgrid_sampling_omp<pcl::PointCloud<pcl::PointXYZ>, pcl::PointCloud<pcl::PointCovariance>>(
        *cloud_in, size_registered_leaf_);
    small_gicp::estimate_covariances_omp(*source_, num_neighbour_, num_threads_);
    source_tree_ = std::make_shared<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>>(
        source_, small_gicp::KdTreeBuilderOMP(num_threads_));
    lock.unlock();
}

void IcpRelocalization::callBackInitPose(const geometry_msgs::msg::PoseWithCovarianceStamped::ConstSharedPtr& msg){
    auto pos = Eigen::Vector3d(msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z);
    auto mat = Eigen::Quaterniond(msg->pose.pose.orientation.w, msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z).matrix();
    result_last_.translation() = pos;
    result_last_.linear() = mat;
    debug_tools::Debug().print("pose:", pos.x(), pos.y(), pos.z(), getYaw(mat));
}

inline void IcpRelocalization::tryRegistration(){
    if(!source_ || !source_tree_) return;
    small_gicp::RegistrationResult result;
    register_->reduction.num_threads = num_threads_;
    register_->rejector.max_dist_sq = max_dist_sq_;
    {
        std::unique_lock<std::mutex> lock(mutex_sensor_);
        result = register_->align(*target_, *source_, *target_tree_, result_last_);
        lock.unlock();
    }
    if(!result.converged){
        debug_tools::Debug().print("icp converge failed...");
        return;
    }
    result_now_ = result.T_target_source;
    tf_.tran = result_now_.translation();
    tf_.yaw = getYaw(result_now_.rotation());
    result_last_ = result.T_target_source;
}

void IcpRelocalization::callBackRegistration(int time_itv){
    while(rclcpp::ok()){
        std::unique_lock<std::mutex> lock(mutex_switch_);
        cv_.wait(lock, [this] { return !stop_flag_; });
        debug_tools::Timer time_begin;
        tryRegistration();
        debug_tools::Timer time_end(time_begin); time_end.log("icp time:", "ms");
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(time_itv));
    }
}

inline void IcpRelocalization::tryPubTransform(){
    // if(result_now_.matrix().isZero()) return;

    // Eigen::Vector3d trans = result_now_.translation();
    // auto rot = Eigen::Quaterniond(result_now_.rotation());
    // auto yaw = getYaw(result_now_.rotation());
    auto trans = tf_.tran;
    auto rot = yawToQuaternion(tf_.yaw);

    nav_msgs::msg::Odometry msg_odom;
    msg_odom.header.stamp = this->now();
    msg_odom.header.frame_id = frame_map_;
    msg_odom.child_frame_id = frame_odom_;

    msg_odom.pose.pose.position.x = trans.x();
    msg_odom.pose.pose.position.y = trans.y();
    msg_odom.pose.pose.position.z = trans.z();
    msg_odom.pose.pose.orientation.x = rot.x();
    msg_odom.pose.pose.orientation.y = rot.y();
    msg_odom.pose.pose.orientation.z = rot.z();
    msg_odom.pose.pose.orientation.w = rot.w();

    pub_odom_->publish(msg_odom);

    geometry_msgs::msg::TransformStamped msg_tf;
    msg_tf.header.stamp = this->now();
    msg_tf.header.frame_id = frame_map_;
    msg_tf.child_frame_id = frame_odom_;

    msg_tf.transform.translation.x = trans.x();
    msg_tf.transform.translation.y = trans.y();
    msg_tf.transform.translation.z = trans.z();
    msg_tf.transform.rotation.x = rot.x();
    msg_tf.transform.rotation.y = rot.y();
    msg_tf.transform.rotation.z = rot.z();
    msg_tf.transform.rotation.w = rot.w();

    tf_broadcaster_->sendTransform(msg_tf);
}

void IcpRelocalization::pubTransform(int time_itv){
    while(rclcpp::ok()){
        tryPubTransform();
        std::this_thread::sleep_for(std::chrono::milliseconds(time_itv));
    }
}

void IcpRelocalization::callBackSwitch(const std_msgs::msg::Bool::ConstSharedPtr& msg){
    debug_tools::Debug().print("[IcpRelocalization switch]:", msg->data);
    stop_flag_ = !stop_flag_;
    cv_.notify_one();
}

IcpRelocalization::IcpRelocalization(): Node("icp_relocalization_sync_node", rclcpp::NodeOptions()){
    auto fov_degree = std::vector<double>{-7, 52, -180.0, 0.0};
    FOV_.clear();
    for(auto& v : fov_degree) FOV_.push_back(v/180.0*M_PI); //角度转弧度

    frame_map_ = declare_parameter<std::string>("frame_map", "map");
    frame_odom_ = declare_parameter<std::string>("frame_odom", "camera_init");
    num_threads_ = declare_parameter<int>("num_threads", 4);
    num_neighbour_ = declare_parameter<int>("num_neighbour", 20);
    size_registered_leaf_ = declare_parameter<float>("size_registered_leaf", 0.25);
    max_dist_sq_ = declare_parameter<float>("max_dist_sq", 2.0);
    auto max_cloud_p_dist = declare_parameter<float>("max_cloud_p_dist", 7.0);
    max_cloud_p_dist_sq_ = std::abs(max_cloud_p_dist * max_cloud_p_dist); 

    result_last_.translation() = Eigen::Vector3d(-9.47, -1.43, 0);
    result_last_.linear() = Eigen::Quaternion<double>(0.71, 0, 0, 0.69).toRotationMatrix();
    tf_.tran = result_last_.translation();
    tf_.yaw = getYaw(result_last_.rotation());
    result_now_.linear() = Eigen::Quaternion<double>(1, 0, 0, 0).toRotationMatrix();

    std::string path_global_pcd = declare_parameter<std::string>("path_global_pcd", "/home/ld/Documents/point_cloud/lab_2f/2025_5_5.pcd");
    cloud_global_map_ = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    if(pcl::io::loadPCDFile<pcl::PointXYZ>(path_global_pcd, *cloud_global_map_) == -1){
        debug_tools::Debug().print("read pcd failed...");
        return;
    }

    register_ = std::make_shared<small_gicp::Registration<small_gicp::GICPFactor, small_gicp::ParallelReductionOMP>>();
    target_ = small_gicp::voxelgrid_sampling_omp<pcl::PointCloud<pcl::PointXYZ>, pcl::PointCloud<pcl::PointCovariance>>(
        *cloud_global_map_, size_registered_leaf_);
    target_tree_ = std::make_shared<small_gicp::KdTree<pcl::PointCloud<pcl::PointCovariance>>>(target_, small_gicp::KdTreeBuilderOMP(num_threads_));
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);

    std::string topic_global_cloud = declare_parameter<std::string>("topic_global_cloud", "/global_cloud");
    pub_global_cloud_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(topic_global_cloud, 10);

    std::string topic_odom_icp = declare_parameter<std::string>("topic_odom_icp", "/icp_odom");
    pub_odom_ = this->create_publisher<nav_msgs::msg::Odometry>(topic_odom_icp, 10);

    group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant); //允许多个回调同时执行
    options_.callback_group = group_;

    std::string topic_cloud_body = declare_parameter<std::string>("topic_cloud_body", "/cloud_registered_body");
    sub_cloud_ = std::make_shared<message_filters::Subscriber<sensor_msgs::msg::PointCloud2>>();
    sub_cloud_->subscribe(this, topic_cloud_body, rmw_qos_profile_sensor_data, options_);

    std::string topic_odom = declare_parameter<std::string>("topic_odom", "/fast_lio_odom");
    sub_odom_ = std::make_shared<message_filters::Subscriber<nav_msgs::msg::Odometry>>();
    sub_odom_->subscribe(this, topic_odom, rmw_qos_profile_sensor_data, options_);

    syn_chronizer_ = std::make_shared<message_filters::TimeSynchronizer<sensor_msgs::msg::PointCloud2, 
            nav_msgs::msg::Odometry>>(*sub_cloud_, *sub_odom_, 10);
    syn_chronizer_->registerCallback(std::bind(&IcpRelocalization::callBackCloudOdom, this, std::placeholders::_1, std::placeholders::_2));

    sub_init_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "/initialpose", 10, std::bind(&IcpRelocalization::callBackInitPose, this, std::placeholders::_1), options_);

    td_icp_ = std::thread(&IcpRelocalization::callBackRegistration, this, 50);
    td_tf_ = std::thread(&IcpRelocalization::pubTransform, this, 20);
    // td_visual_ = std::thread([this](){
    //     bool has_pub_cloud{false}; //只发布一次
    //     while(rclcpp::ok()){
    //         if(!has_pub_cloud && pub_global_cloud_->get_subscription_count() >= 1){
    //             sensor_msgs::msg::PointCloud2 msg_cloud;
    //             pcl::toROSMsg(*cloud_global_map_, msg_cloud);
    //             msg_cloud.header.stamp = this->now();
    //             msg_cloud.header.frame_id = frame_map_;
    //             pub_global_cloud_->publish(msg_cloud);
    //             has_pub_cloud = true;
    //         }
    //         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //     }
    // });

    sub_switch_ = this->create_subscription<std_msgs::msg::Bool>(
        "/icp_switch", 10, std::bind(&IcpRelocalization::callBackSwitch, this, std::placeholders::_1));
}

IcpRelocalization::~IcpRelocalization(){
    {  
        std::lock_guard<std::mutex> lock(mutex_switch_);  
        stop_flag_ = false; // 停止运行标志  
    }  
    cv_.notify_one(); //结束线程前先解锁，否则一直堵塞，无法退出
    
    if(td_icp_.joinable()) td_icp_.join();
    if(td_tf_.joinable()) td_tf_.join();
    // if(td_visual_.joinable()) td_visual_.join();
    debug_tools::Debug().print("~IcpRelocalization");
}
} //namespace icp_relocalization

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto icp_node = std::make_shared<icp_relocalization::IcpRelocalization>();
    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 4);
    executor.add_node(icp_node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}