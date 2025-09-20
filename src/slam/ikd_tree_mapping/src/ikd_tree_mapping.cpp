#include "ikd_tree_mapping/ikd_tree_mapping.h"

namespace ikd_tree_mapping{

inline bool IkdTreeMapping::checkFOV(const Eigen::Vector<float, 6>& range_FOV, const pcl::PointXYZ& p){
    auto distance = p.x*p.x + p.y*p.y + p.z*p.z;
    if(distance <= range_FOV[4] || distance > range_FOV[5]) return false;
    float hor = std::atan2(p.y, p.x);
    float ver = std::atan2(p.z, std::sqrt((std::pow(p.x, 2) + std::pow(p.y, 2)))); 
    if(hor < range_FOV(2) || hor > range_FOV(3)) return false;
    if(ver < range_FOV(0) || ver > range_FOV(1)) return false;
    return true;
}

void IkdTreeMapping::callBackCloudOdom(const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg_cloud, const nav_msgs::msg::Odometry::ConstSharedPtr& msg_odom){
    // debug_tools::Timer t_0;
    auto cloud_in = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    pcl::fromROSMsg(*msg_cloud, *cloud_in);
    if(cloud_in->points.size() <= 0) return;

    auto odom_p = Eigen::Vector3f(msg_odom->pose.pose.position.x, msg_odom->pose.pose.position.y, msg_odom->pose.pose.position.z);
    auto odom_m = Eigen::Quaternionf(msg_odom->pose.pose.orientation.w, msg_odom->pose.pose.orientation.x, 
        msg_odom->pose.pose.orientation.y, msg_odom->pose.pose.orientation.z).matrix();

    group_visual.odom_pos_for_visual = odom_p;

    std::for_each(std::execution::par, cloud_in->points.begin(), cloud_in->points.end(), [&](pcl::PointXYZ& p){
        if(!checkFOV(cfg_.range_FOV, p)) p.x = p.y = p.z = NAN;
        else{
            Eigen::Vector3f p_origin(p.x, p.y, p.z);
            Eigen::Vector3f p_world = odom_m * (cfg_.ext_m*p_origin + cfg_.ext_p) + odom_p;
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

    static bool is_first{true};
    if(is_first){
        kd_tree_->Build(cloud_in->points);
        is_first = false;
    }
    else kd_tree_->Add_Points(cloud_in->points, true);
    pubTransform(odom_p);
    // debug_tools::Timer t_1(t_0); t_1.log("callBackCloudOdom", "ms");
}

Eigen::Quaternionf IkdTreeMapping::eulerToQuaternion(float roll, float pitch, float yaw){
        // 将欧拉角转换为四元数  
    // 注意：Eigen 使用弧度而非角度  
    Eigen::AngleAxisf rollAngle(roll, Eigen::Vector3f::UnitX());  
    Eigen::AngleAxisf pitchAngle(pitch, Eigen::Vector3f::UnitY());  
    Eigen::AngleAxisf yawAngle(yaw, Eigen::Vector3f::UnitZ());  

    // 合成四元数，顺序为先 yaw、后 pitch、再 roll  
    Eigen::Quaternionf q = yawAngle * pitchAngle * rollAngle;  
    return q;  
}

void IkdTreeMapping::pubMap(rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr& pub, 
        std::vector<pcl::PointXYZ, Eigen::aligned_allocator<pcl::PointXYZ>>& points){
    auto cloud = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
    cloud->assign(points.begin(), points.end());
    sensor_msgs::msg::PointCloud2 msg_cloud;
    pcl::toROSMsg(*cloud, msg_cloud);
    msg_cloud.header.frame_id = cfg_.frame_odom;
    msg_cloud.header.stamp = this->now();
    pub->publish(msg_cloud);
}

void IkdTreeMapping::pubFOV(Eigen::Vector<float, 6>& fov, rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr& pub){
    visualization_msgs::msg::Marker marker;  
    marker.header.frame_id = cfg_.frame_lidar;
    marker.header.stamp = this->now();  
    marker.ns = "fov";  
    marker.id = 0;  
    marker.type = visualization_msgs::msg::Marker::LINE_LIST; // 使用线段列表  
    marker.scale.x = 0.02; // 线宽  
    marker.color.r = 77/256.0;  
    marker.color.g = 56/256.0;  
    marker.color.b = 243/256.0;  
    marker.color.a = 1.0;  

    double max_range_dis = std::sqrt(fov[5]);

    geometry_msgs::msg::Point top_left, top_right, bottom_left, bottom_right;   // 计算矩形的四个定点 
    float top_z = max_range_dis * sin(fov[1]);   // 垂直方向最大角度  
    float bottom_z = max_range_dis * sin(fov[0]); // 垂直方向最小角度 
    float horizontal_angle_left = fov[2]; // 水平最小角度  
    float horizontal_angle_right = fov[3]; // 水平最大角度  
    
    top_left.x = max_range_dis * cos(horizontal_angle_left);  
    top_left.y = max_range_dis * sin(horizontal_angle_left);  
    top_left.z = top_z; // 上边的Z坐标  
    
    top_right.x = max_range_dis * cos(horizontal_angle_right);  
    top_right.y = max_range_dis * sin(horizontal_angle_right);  
    top_right.z = top_z; // 上边的Z坐标  
    
    bottom_left.x = max_range_dis * cos(horizontal_angle_left);  
    bottom_left.y = max_range_dis * sin(horizontal_angle_left);  
    bottom_left.z = bottom_z; // 下边的Z坐标  
    
    bottom_right.x = max_range_dis * cos(horizontal_angle_right);  
    bottom_right.y = max_range_dis * sin(horizontal_angle_right);  
    bottom_right.z = bottom_z; // 下边的Z坐标  

    geometry_msgs::msg::Point origin; origin.x = 0, origin.y = 0, origin.z = 0;
    marker.points.push_back(origin); // 原点  
    marker.points.push_back(top_left);  
    marker.points.push_back(origin); // 原点  
    marker.points.push_back(top_right);  
    marker.points.push_back(origin); // 原点  
    marker.points.push_back(bottom_left);  
    marker.points.push_back(origin); // 原点  
    marker.points.push_back(bottom_right);  

    pub->publish(marker);
}

ikdtreeNS::BoxPointType IkdTreeMapping::getSmallBox(ikdtreeNS::BoxPointType& box_in, float rate){
    ikdtreeNS::BoxPointType box_out = box_in;
    for(int i{0}; i < 3; i++){
        box_out.vertex_min[i] *= rate;
        box_out.vertex_max[i] *= rate;
    }
    return box_out;
}

inline void IkdTreeMapping::pubTransform(const Eigen::Vector3f& tran){
    geometry_msgs::msg::TransformStamped msg_tf;
    msg_tf.header.stamp = this->now();
    msg_tf.header.frame_id = cfg_.frame_odom;
    msg_tf.child_frame_id = "g_" + cfg_.frame_lidar;

    msg_tf.transform.translation.x = tran.x();
    msg_tf.transform.translation.y = tran.y();
    msg_tf.transform.translation.z = tran.z();

    msg_tf.transform.rotation.w = 1;
    msg_tf.transform.rotation.x = 0;
    msg_tf.transform.rotation.y = 0;
    msg_tf.transform.rotation.z = 0;

    group_visual.tf_broadcaster_->sendTransform(msg_tf);
}

IkdTreeMapping::IkdTreeMapping(): Node("ikd_tree_mapping_node", rclcpp::NodeOptions()){
    cfg_.ikd_delete_param = declare_parameter<float>("ikd_delete_param", 0.3);
    cfg_.ikd_balance_param = declare_parameter<float>("ikd_balance_param", 0.6);
    cfg_.ikd_box_length = declare_parameter<float>("resolution", 0.01);
    kd_tree_ = std::make_shared<ikdtreeNS::KD_TREE<pcl::PointXYZ>>(cfg_.ikd_delete_param, cfg_.ikd_balance_param, cfg_.ikd_box_length);

    cfg_.frame_lidar = declare_parameter<std::string>("frame_lidar", "body");
    cfg_.frame_odom = declare_parameter<std::string>("frame_odom", "camera_init");

    auto ext_tran = declare_parameter<std::vector<float>>("extrinsic_T", std::vector<float>({0.0, 0.0, 0.0}));
    auto ext_eul = declare_parameter<std::vector<float>>("extrinsic_Euler", std::vector<float>({0.0, 0.0, 0.0})); //角度制 roll pitch yaw
    cfg_.ext_p = Eigen::Vector3f(ext_tran[0], ext_tran[1], ext_tran[2]);
    cfg_.ext_m = eulerToQuaternion(ext_eul[0], ext_eul[1], ext_eul[2]).matrix();

    auto range_mapping_vec = declare_parameter<std::vector<float>>("range_mapping", std::vector<float>({-20.0, 20.0, -20.0, 20.0, -2.0, 4.0}));  //单位m
    cfg_.range_mapping = range_mapping_vec;

    auto range_visual_vec = declare_parameter<std::vector<float>>("range_visual", std::vector<float>({-4.0, 4.0, -4.0, 4.0, -2.0, 2.0}));
    cfg_.range_visual = range_visual_vec;

    auto range_FOV_vec = declare_parameter<std::vector<float>>("range_FOV", std::vector<float>{-7.0, 52.0, -45.0, 45.0, 0.15, 10.0}); // 角度制
    cfg_.range_FOV = Eigen::Vector<float, 6>(range_FOV_vec[0]/180.0*M_PI, range_FOV_vec[1]/180.0*M_PI, range_FOV_vec[2]/180.0*M_PI, 
                range_FOV_vec[3]/180.0*M_PI, range_FOV_vec[4]*range_FOV_vec[4], range_FOV_vec[5]*range_FOV_vec[5]);

    group_add_.topic_cloud = declare_parameter<std::string>("topic_cloud_in_add", "/cloud_registered_body");
    group_add_.sub_cloud = std::make_shared<message_filters::Subscriber<sensor_msgs::msg::PointCloud2>>();
    group_add_.sub_cloud->subscribe(this, group_add_.topic_cloud);
    group_add_.topic_odom = declare_parameter<std::string>("topic_odom", "/fast_lio_odom");
    group_add_.sub_odom = std::make_shared<message_filters::Subscriber<nav_msgs::msg::Odometry>>();
    group_add_.sub_odom->subscribe(this, group_add_.topic_odom);
    group_add_.syn_chronizer = std::make_shared<message_filters::TimeSynchronizer<sensor_msgs::msg::PointCloud2, 
            nav_msgs::msg::Odometry>>(*group_add_.sub_cloud , *group_add_.sub_odom, 10);
    group_add_.syn_chronizer->registerCallback(std::bind(&IkdTreeMapping::callBackCloudOdom, this, std::placeholders::_1, std::placeholders::_2));

    group_visual.topic_map = declare_parameter<std::string>("topic_map_visual", "/ikd_mapping_result");
    group_visual.pub_map_visual = this->create_publisher<sensor_msgs::msg::PointCloud2>(group_visual.topic_map, 10);

    cfg_.is_pub_FOV = declare_parameter<bool>("is_pub_FOV", true);
    group_visual.topic_FOV = declare_parameter<std::string>("topic_FOV_visual", "/FOV_range_visual");
    if(cfg_.is_pub_FOV) group_visual.pub_FOV = this->create_publisher<visualization_msgs::msg::Marker>(group_visual.topic_FOV, 10);

    group_visual.mil_timer = declare_parameter<float>("visual_mil_timer", 200);
    group_visual.mil_time_thres = declare_parameter<float>("visual_mil_time_thres", 10);

    group_visual.tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);

    std::string topic_save_pcd = declare_parameter<std::string>("topic_save_pcd", "/ikd_mapping_save_pcd");
    service_save_pcd_ = this->create_service<rm_interface::srv::SavePCD>(topic_save_pcd, 
        [this](const std::shared_ptr<rm_interface::srv::SavePCD::Request> request, const std::shared_ptr<rm_interface::srv::SavePCD::Response> response){
            std::vector<pcl::PointXYZ, Eigen::aligned_allocator<pcl::PointXYZ>> points;
            kd_tree_->Box_Search(cfg_.range_mapping, points);
            auto cloud = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
            for(auto& p : points) cloud->push_back(p);
            pcl::PCDWriter writer;
            response->point_num = cloud->size();
            response->memory_gb = static_cast<float>(cloud->points.size() * sizeof(pcl::PointXYZ)/1024.0/1024.0/1024.0);
            if(cloud->size() > 0) response->result = writer.write<pcl::PointXYZ>(request->file_path, *cloud);
        }
    );

    group_visual.timer_visual = this->create_wall_timer(std::chrono::milliseconds(static_cast<uint64_t>(group_visual.mil_timer)),[this](){
        debug_tools::Timer t_0;
        std::vector<pcl::PointXYZ, Eigen::aligned_allocator<pcl::PointXYZ>> points;
        auto p_odom = group_visual.odom_pos_for_visual;

        ikdtreeNS::BoxPointType box_visual = std::vector<float>({cfg_.range_visual.vertex_min[0]+p_odom.x(), cfg_.range_visual.vertex_max[0]+p_odom.x(),
            cfg_.range_visual.vertex_min[1]+p_odom.y(), cfg_.range_visual.vertex_max[1]+p_odom.y(), 
            cfg_.range_visual.vertex_min[2]+p_odom.z(), cfg_.range_visual.vertex_max[2]+p_odom.z()});

        kd_tree_->Box_Search(box_visual, points);
        pubMap(group_visual.pub_map_visual, points);
        if(cfg_.is_pub_FOV) pubFOV(cfg_.range_FOV, group_visual.pub_FOV);
        debug_tools::Debug().print("point size:", points.size(), kd_tree_->size());
        debug_tools::Timer t_1(t_0); t_1.log("visual", "ms");
    });
}
}

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto ikd_tree_mapping_node = std::make_shared<ikd_tree_mapping::IkdTreeMapping>();
    rclcpp::spin(ikd_tree_mapping_node);
    rclcpp::shutdown();
    return 0;
}