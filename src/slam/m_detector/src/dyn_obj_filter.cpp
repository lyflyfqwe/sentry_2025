#include "m_detector/dyn_obj_filter.h"

namespace m_detector{

DynObjFilter::DynObjFilter(std::string node_name, rclcpp::NodeOptions option): Node(node_name, option){
    ros_config_ = std::make_shared<Config>(this, "");
    max_ind_ = std::floor(M_PI * 2 / ros_config_->hor_resolution_max_);
    if(pcl_his_list_.size() == 0){
        auto first_frame = std::make_shared<pcl::PointCloud<pcl::PointXYZINormal>>();
        first_frame->reserve(400000);
        pcl_his_list_.push_back(first_frame);
        laser_cloud_dyn_obj_ = std::make_shared<pcl::PointCloud<pcl::PointXYZINormal>>();
        laser_cloud_dyn_obj_world_ = std::make_shared<pcl::PointCloud<pcl::PointXYZINormal>>();
        laser_cloud_stead_obj_ = std::make_shared<pcl::PointCloud<pcl::PointXYZINormal>>();
        
        int xy_index[3] = {-1, 1};
        for(int index_hor{0}; index_hor < 2*hor_num_+1; index_hor++){
            for(int index_vert{0}; index_vert < 2*ver_num_+1; index_vert++){
                int value = (index_hor/2 + index_hor%2) * xy_index[index_hor%2] * MAX_1D_HALF
                    + (index_vert/2 + index_vert%2) * xy_index[index_vert%2];
                pos_offset_.push_back(value);
            }
        }
    }

    map_cons_hor_num1_ = std::ceil(ros_config_->map_cons_hor_thr1_ / ros_config_->hor_resolution_max_);
    map_cons_ver_num1_ = std::ceil(ros_config_->map_cons_ver_thr1_ / ros_config_->ver_resolution_max_);

    map_cons_hor_num2_ = std::ceil(ros_config_->map_cons_hor_thr2_ / ros_config_->hor_resolution_max_);
    map_cons_ver_num2_ = std::ceil(ros_config_->map_cons_ver_thr2_ / ros_config_->ver_resolution_max_);

    map_cons_hor_num3_ = std::ceil(ros_config_->map_cons_hor_thr3_ / ros_config_->hor_resolution_max_);
    map_cons_ver_num3_ = std::ceil(ros_config_->map_cons_ver_thr3_ / ros_config_->ver_resolution_max_);

    interp_hor_num_ = std::ceil(ros_config_->interp_hor_thr_ / ros_config_->hor_resolution_max_);
    interp_ver_num_ = std::ceil(ros_config_->interp_ver_thr_ / ros_config_->ver_resolution_max_);

    occ_hor_num2_ = std::ceil(ros_config_->occ_hor_thr2_ / ros_config_->hor_resolution_max_);
    occ_ver_num2_ = std::ceil(ros_config_->occ_ver_thr2_ / ros_config_->ver_resolution_max_);

    occ_hor_num3_ = std::ceil(ros_config_->occ_hor_thr3_ / ros_config_->hor_resolution_max_);
    occ_ver_num3_ = std::ceil(ros_config_->occ_ver_thr3_ / ros_config_->ver_resolution_max_);

    depth_cons_hor_num2_ = std::ceil(ros_config_->depth_cons_hor_thr2_ / ros_config_->hor_resolution_max_);
    depth_cons_ver_num2_ = std::ceil(ros_config_->depth_cons_ver_thr2_ / ros_config_->ver_resolution_max_);

    depth_cons_hor_num3_ = std::ceil(ros_config_->depth_cons_hor_thr3_ / ros_config_->hor_resolution_max_);
    depth_cons_ver_num3_ = std::ceil(ros_config_->depth_cons_ver_thr3_ / ros_config_->ver_resolution_max_);

    buffer_.init(ros_config_->buffer_size_);

    pixel_fov_up_ = std::floor((ros_config_->fov_up_/180.0*M_PI + 0.5*M_PI) /  ros_config_->ver_resolution_max_);
    pixel_fov_down_ = std::floor((ros_config_->fov_down_/180.0*M_PI + 0.5*M_PI) /  ros_config_->ver_resolution_max_);
    pixel_fov_cut_ = std::floor((ros_config_->fov_cut_/180.0*M_PI + 0.5*M_PI) /  ros_config_->ver_resolution_max_);
    pixel_fov_left_ = std::floor((ros_config_->fov_left_/180.0*M_PI + 0.5*M_PI) /  ros_config_->hor_resolution_max_);
    pixel_fov_right_ = std::floor((ros_config_->fov_right_/180.0*M_PI + 0.5*M_PI) /  ros_config_->hor_resolution_max_);

    max_pointers_num_ = 
        std::round((ros_config_->max_depth_map_num_ * ros_config_->depth_map_dur_ + ros_config_->buffer_delay_) / ros_config_->frame_dur_) + 1;
    point_soph_pointers_.reserve(max_pointers_num_);

    std::cout << "[DynObjFilter] max_pointers_num_: " << max_pointers_num_ << "\n";

    auto time_malloc_begin = std::chrono::system_clock::now();
    for(int i{0}; i < max_pointers_num_; i++){
        PointSoph *p = new PointSoph[ros_config_->points_num_perframe_];
        point_soph_pointers_.push_back(p);
    }
    // dyn_obj_cluster_.init();

    auto time_mallco_end = std::chrono::system_clock::now() - time_malloc_begin;
    RCLCPP_INFO(this->get_logger(), "init finished, consumed time: %ldms", 
            std::chrono::duration_cast<std::chrono::milliseconds>(time_mallco_end).count());

    msg_trans_.reset(new sensor_msgs::msg::PointCloud2);

    const auto addField = [this](const std::string& name, const int offset, const uint8_t datatype) {
        sensor_msgs::msg::PointField field;
        field.name = name;
        field.offset = offset;
        field.datatype = datatype;
        field.count = 1;
        msg_trans_->fields.push_back(field);
    };

    addField("x", 0, sensor_msgs::msg::PointField::FLOAT32);
    addField("y", msg_trans_->fields.back().offset + sizeof(float), sensor_msgs::msg::PointField::FLOAT32);
    addField("z", msg_trans_->fields.back().offset + sizeof(float), sensor_msgs::msg::PointField::FLOAT32);
    addField("t", msg_trans_->fields.back().offset + sizeof(float), sensor_msgs::msg::PointField::UINT32);
    addField("intensity", msg_trans_->fields.back().offset + sizeof(std::uint32_t), sensor_msgs::msg::PointField::FLOAT32);
    addField("tag", msg_trans_->fields.back().offset + sizeof(float), sensor_msgs::msg::PointField::UINT8);
    addField("line", msg_trans_->fields.back().offset + sizeof(std::uint8_t), sensor_msgs::msg::PointField::UINT8);
    msg_trans_->is_bigendian = false;
    msg_trans_->point_step = sizeof(float) * 4 + sizeof(uint32_t) + sizeof(uint8_t) * 2;
    msg_trans_->is_dense = true;

    pub_cloud_body_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/m_detector_cloud_body", 1000);

    pub_cloud_world_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/m_detector_cloud_world", 1000);

    sub_cloud_body_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        ros_config_->topic_cloud_body_, 
        rclcpp::SensorDataQoS(), 
        [this](const sensor_msgs::msg::PointCloud2::SharedPtr msg){
            std::shared_ptr<pcl::PointCloud<pcl::PointXYZINormal>> cloud_in(new pcl::PointCloud<pcl::PointXYZINormal>());
            pcl::fromROSMsg(*msg, *cloud_in);
            std::cout << "cloud in size: " << cloud_in->size() << "\n";
            this->buffer_cloud_.push_back(cloud_in);
        }
    );

    // sub_pcl_livox_ = this->create_subscription<livox_ros_driver2::msg::CustomMsg>(
    //     "/livox/lidar", rclcpp::SensorDataQoS(),
    //     [this](const livox_ros_driver2::msg::CustomMsg::SharedPtr msg){
            
    //         auto time_begin = std::chrono::steady_clock::now();
            
    //         std::shared_ptr<pcl::PointCloud<pcl::PointXYZINormal>> cloud_in(new pcl::PointCloud<pcl::PointXYZINormal>());
    //         convertCustomMsg2PointCloud(msg, cloud_in);

    //         this->buffer_cloud_.push_back(cloud_in);

    //         double time_consumed = std::chrono::duration_cast<std::chrono::milliseconds>(
    //                 std::chrono::steady_clock::now() - time_begin).count();
                    
    //         std::cout << "livox msg size:" << msg->points.size() << " ";
    //         std::cout << "livox cloud size: " << cloud_in->size() << " " << time_consumed << "ms\n";

    //         sensor_msgs::msg::PointCloud2 msg_pub;
    //         pcl::toROSMsg(*cloud_in, msg_pub);

    //         msg_pub.header.frame_id = "body";
    //         msg_pub.header.stamp = msg->header.stamp;
    //         pub_cloud_body_->publish(msg_pub);
    //     }
    // );

    sub_odom_body_ = this->create_subscription<nav_msgs::msg::Odometry>(
        ros_config_->topic_odom_body_,
        rclcpp::SensorDataQoS(),
        [this](const nav_msgs::msg::Odometry::SharedPtr msg){
            Eigen::Quaterniond q;
            q.w() = msg->pose.pose.orientation.w;
            q.x() = msg->pose.pose.orientation.x;
            q.y() = msg->pose.pose.orientation.y;
            q.z() = msg->pose.pose.orientation.z;

            Eigen::Matrix3d cur_rot = q.matrix();
            this->buffer_rot_.push_back(cur_rot);

            Eigen::Vector3d cur_pos;
            cur_pos << msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z;
            this->buffer_pos_.push_back(cur_pos);

            // std::cout << cur_pos << "\n";

            double cur_time = msg->header.stamp.sec;
            this->buffer_time_.push_back(cur_time);
        }
    );

    timer_ = this->create_wall_timer(std::chrono::milliseconds(10), [this](){
        if(this->buffer_cloud_.size() <= 0 || this->buffer_rot_.size() <= 0 
                || this->buffer_pos_.size() <= 0 || this->buffer_time_.size() <= 0) return;
        
        auto cloud = buffer_cloud_.at(0);
        buffer_cloud_.pop_front();

        auto rot = buffer_rot_.at(0);
        buffer_rot_.pop_front();

        auto pos = buffer_pos_.at(0);
        buffer_pos_.pop_front();

        auto time = buffer_time_.at(0);
        buffer_time_.pop_front();

        filter(cloud, rot, pos, time);

        sensor_msgs::msg::PointCloud2 msg_pub_body;
        pcl::toROSMsg(*laser_cloud_dyn_obj_, msg_pub_body);
        msg_pub_body.header.frame_id = "body";
        pub_cloud_body_->publish(msg_pub_body);

        sensor_msgs::msg::PointCloud2 msg_pub_world;
        pcl::toROSMsg(*laser_cloud_dyn_obj_world_, msg_pub_world);
        msg_pub_world.header.frame_id = "camera_init";
        pub_cloud_world_->publish(msg_pub_world);
    });
}

void DynObjFilter::convertCustomMsg2CloudMsg(const livox_ros_driver2::msg::CustomMsg::SharedPtr& msg_in, 
        sensor_msgs::msg::PointCloud2::SharedPtr& cloud_out){
    
    cloud_out->header = msg_in->header;
    cloud_out->width = msg_in->point_num;
    cloud_out->height = 1;
    cloud_out->row_step = msg_in->point_num * cloud_out->point_step;
    cloud_out->data.resize(cloud_out->row_step);

    unsigned char* datas_ptr = cloud_out->data.data();

    for(int i{0}; i < static_cast<int>(msg_in->point_num); i++){
        
        *reinterpret_cast<float*>(datas_ptr + cloud_out->fields[0].offset) = msg_in->points[i].x;
        *reinterpret_cast<float*>(datas_ptr + cloud_out->fields[1].offset) = msg_in->points[i].y;
        *reinterpret_cast<float*>(datas_ptr + cloud_out->fields[2].offset) = msg_in->points[i].z;
        *reinterpret_cast<std::uint32_t*>(datas_ptr + cloud_out->fields[3].offset) = msg_in->points[i].offset_time;
        *reinterpret_cast<float*>(datas_ptr + cloud_out->fields[4].offset) = msg_in->points[i].reflectivity;
        *reinterpret_cast<std::uint8_t*>(datas_ptr + cloud_out->fields[5].offset) = msg_in->points[i].tag;
        *reinterpret_cast<std::uint8_t*>(datas_ptr + cloud_out->fields[6].offset) = msg_in->points[i].line;

        datas_ptr += cloud_out->point_step;
    } 
}

void DynObjFilter::convertCustomMsg2PointCloud(const livox_ros_driver2::msg::CustomMsg::SharedPtr& msg_in, 
        pcl::PointCloud<pcl::PointXYZINormal>::Ptr& cloud_out){

    cloud_out->clear();
    int msg_size = msg_in->point_num;
    cloud_out->reserve(msg_size);

    pcl::PointXYZINormal p;
    for(int i{0}; i <  msg_size; i++){
        if(msg_in->points[i].x * msg_in->points[i].x 
                + msg_in->points[i].y * msg_in->points[i].y 
                + msg_in->points[i].z * msg_in->points[i].z  > 25) continue;

        p.x = msg_in->points[i].x;
        p.y = msg_in->points[i].y;
        p.z = msg_in->points[i].z;
        p.intensity = msg_in->points[i].reflectivity;
        p.curvature = msg_in->points[i].offset_time / float(1000000);

        cloud_out->push_back(p);
    }
}

bool DynObjFilter::invalidPointCheck(const Eigen::Vector3d& point){
    // std::cout << "invalidPointCheck ";
    // return (std::pow(point(0), 2) + std::pow(point(1), 2) + std::pow(point(2), 2) < ros_config_->blind_dis_ * ros_config_->blind_dis_)
    //     && (std::fabs(point(2)) < 0.1)
    //     || (ros_config_->dataset_ == 1 && std::fabs(point(0)) < 0.1 && std::fabs(point(1)) < 1.0);

    //点距离在小于盲区， 即dis^2 < blind_dis^2
    bool a = std::pow(point(0), 2) + std::pow(point(1), 2) + std::pow(point(2), 2) < ros_config_->blind_dis_ * ros_config_->blind_dis_;
    //点z轴坐标 小于 0.1
    bool b = std::fabs(point(2)) < 0.1;

    bool c = ros_config_->dataset_ == 1 && std::fabs(point(0)) < 0.1 && std::fabs(point(1)) < 1.0;

    return (a && b) || c;
}

bool DynObjFilter::selfPointCheck(const Eigen::Vector3d& point){
    // std::cout << "selfPointCheck ";
    if(ros_config_->dataset_ != 0) return false;
    return (point(0) > -1.2 && point(0) < -0.4 && point(1) > -1.7 && point(1) < -1.0 && point(2) > -0.65 && point(2) < -0.4)
        || (point(0) > -1.75 && point(0) < -0.85 && point(1) > 1.0 && point(1) < 1.6 && point(2) > -0.75 && point(2) < -0.40) 
        || (point(0) > 1.4 && point(0) < 1.7 && point(1) > -1.3 && point(1) < -0.9 && point(2) > -0.8 && point(2) < -0.6) 
        || (point(0) > 2.45 && point(0) < 2.6 && point(1) > -0.6 && point(1) < -0.45 && point(2) > -1.0 && point(2) < -0.9) 
        || (point(0) > 2.45 && point(0) < 2.6 && point(1) > 0.45 && point(1) < 0.6 && point(2) > -1.0 && point(2) < -0.9);
}

void DynObjFilter::sphericalProjection(PointSoph& p, int depth_index, const Eigen::Matrix3d& rot, const Eigen::Vector3d& trans, PointSoph& p_spherical){
    if(std::fabs(p.last_vecs_.at(depth_index % HASH_PRIM)[2]) > 10E-5){ //已初始化，采用初始化的值

        p_spherical.vec_ = p.last_vecs_.at(depth_index % HASH_PRIM);

        p_spherical.hor_ind_ = p.last_positions_.at(depth_index % HASH_PRIM)[0];
        p_spherical.ver_ind_ = p.last_positions_.at(depth_index % HASH_PRIM)[1];
        p_spherical.position_ = p.last_positions_.at(depth_index % HASH_PRIM)[2];
    }
    else{ //未初始化
        Eigen::Vector3d p_proj = rot * (p.glob_ - trans);
        p_spherical.getVec(p_proj, ros_config_->hor_resolution_max_, ros_config_->ver_resolution_max_);
        p.last_vecs_.at(depth_index % HASH_PRIM) = p_spherical.vec_;

        p.last_positions_.at(depth_index % HASH_PRIM)[0] = p_spherical.hor_ind_;
        p.last_positions_.at(depth_index % HASH_PRIM)[1] = p_spherical.ver_ind_;
        p.last_positions_.at(depth_index % HASH_PRIM)[2] = p_spherical.position_;
    }
}

bool DynObjFilter::checkVerFOV(const PointSoph& p, const DepthMap& map){
    bool ver_up{false}, ver_down{false};

    for(int i = p.ver_ind_; i >= pixel_fov_down_; i--){ //检查像素左侧 是否有存在已更新的像素
        int cur_pos = p.hor_ind_ * MAX_1D_HALF + i;
        if(map.depth_map_[cur_pos].size() > 0){
            ver_up = true;
            break;
        }
    }
    for(int i = p.ver_ind_; i <= pixel_fov_up_; i++){  //检查像素右侧 是否有存在已更新的像素
        int cur_pos = p.hor_ind_ * MAX_1D_HALF + i;
        if(map.depth_map_[cur_pos].size() > 0){
            ver_down = true;
            break;
        }
    }
    return !(ver_up && ver_down); //如果左右均未更新，则返回true
}

void DynObjFilter::checkNeighbor(const PointSoph& p, const DepthMap& map, float& min_depth, float& max_depth){
    int n = ros_config_->check_neighbor_range_; //默认值1

    //检查四周八个格子
    for(int i = -n; i <= n; i++){
        for(int j = -n; j <= n; j++){
            int cur_pos = MAX_1D_HALF * (p.hor_ind_ + i) + (p.ver_ind_ + j); 

            //检查越界，和depthmap已更新
            if(cur_pos >= 0 && cur_pos < MAX_2D_N && map.depth_map_[cur_pos].size() > 0){
                float min_depth_cur = map.min_depth_static_[cur_pos];
                float max_depth_cur = map.max_depth_static_[cur_pos];
                
                if(min_depth > 10E-5)
                    min_depth = std::min(min_depth, min_depth_cur);
                else min_depth = min_depth_cur;

                if(max_depth > 10E-5)
                    max_depth = std::max(max_depth, max_depth_cur);
                else max_depth = max_depth_cur;
            }
        }
    }
}

bool DynObjFilter::enterCase_1(const PointSoph& p, const DepthMap& map){
    float min_depth{0}, max_depth{0};
    if(map.depth_map_[p.position_].size() > 0){ //深度图第p.position_像素的最大最小深度 已存在
        min_depth = map.min_depth_static_[p.position_];
        max_depth = map.max_depth_static_[p.position_];
    }
    else if(p.ver_ind_ <= pixel_fov_up_ && p.ver_ind_ > pixel_fov_down_ 
        && p.hor_ind_ <= pixel_fov_left_ && p.hor_ind_ >= pixel_fov_right_
            && checkVerFOV(p, map)){  //不存在， 检查是否越界
        checkNeighbor(p, map, min_depth, max_depth);   
    }

    float cur_min = std::max(cutoff_value_, ros_config_->k_depth_min_thr1_ * (p.vec_(2) - ros_config_->d_depth_min_thr1_))
            + ros_config_->enter_min_thr1_; //+0.5
    float cur_max = std::max(cutoff_value_, ros_config_->k_depth_max_thr1_ * (p.vec_(2) - ros_config_->d_depth_max_thr1_))
            + ros_config_->enter_max_thr1_; //+0.5
    float cur_depth = ros_config_->depth_thr1_; //0.5

    if(ros_config_->dataset_ == 0 && p.is_distort_){
        cur_min *= enlarge_distort_;
        cur_max *= enlarge_distort_;
        cur_depth *= enlarge_distort_;
    }

    if(p.vec_(2) < min_depth - cur_min  //当前像素的深度 小于历史最小深度，即当前的像素遮挡背景
        || (min_depth < p.vec_(2) - cur_min && max_depth > p.vec_(2) + cur_max) //当前像素深度大于历史最小深度，小于历史最大深度
        || (ros_config_->stop_object_detect_ && min_depth < 10E-5 && max_depth < 10E-5 //stop_object_detect_ = false 不会发生
                && map.depth_map_[p.position_].size() > 0
                && p.vec_(2) < map.max_depth_all_[p.position_] + 1.0)){
        case1_num_++;
        return true;
    }
    return false;
}

float DynObjFilter::depthMapInterpolationStatic(PointSoph& p, int map_index, const std::vector<std::vector<PointSoph*>>& depth_map){
    if(std::fabs(p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_)) > 10E-4)
        return p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_); //depth_cal
    
    Eigen::Vector3f p_1 = Eigen::Vector3f::Zero();
    Eigen::Vector3f p_2 = Eigen::Vector3f::Zero();
    Eigen::Vector3f p_3 = Eigen::Vector3f::Zero();
    std::vector<Eigen::Vector3f> p_neighbors;
    int all_num {0}, static_num{0}, no_bg_num{0};
    
    for(int index_hor = -interp_hor_num_; index_hor <= interp_hor_num_; index_hor++){
        for(int index_ver = -interp_ver_num_; index_ver <= interp_ver_num_; index_ver++){
            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            const std::vector<PointSoph*> &point_in_pixel = depth_map[pos_new];
            for(size_t j{0}; j < point_in_pixel.size(); j++){
                const PointSoph* point = point_in_pixel[j];

                if(std::fabs(point->time_ - p.time_) < ros_config_->frame_dur_) continue;

                float hor_minus = point->vec_(0) - p.vec_(0);
                float ver_minus = point->vec_(1) - p.vec_(1);
                
                //for calculate 3 num
                if(std::fabs(hor_minus) < ros_config_->interp_hor_thr_ && std::fabs(ver_minus) < ros_config_->interp_ver_thr_){
                    all_num++;
                    if(point->dyn_flag_ == DynObjFlag::STATIC)
                        static_num++;
                    
                    if(point->vec_(2) - p.vec_(2) <= ros_config_->interp_static_max_ && p.vec_(2) - point->vec_(2) < 5.0)
                        no_bg_num++;

                    if(point->dyn_flag_ == DynObjFlag::STATIC){
                        p_neighbors.push_back(point->vec_);
                        if(p_1(2)<0.000001 
                                || std::fabs(hor_minus) + std::fabs(ver_minus) < std::fabs(p_1(0)-p.vec_(0)) + std::fabs(p_1(1) - p.vec_(1)))
                            p_1 = point->vec_;
                    }
                }
            }
        }
    }

    if(p_1(2) < 10E-5){
        p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_) = -1;
        return -1;
    }

    size_t neibor_size = p_neighbors.size();
    for(size_t t_i{0}; t_i < neibor_size; t_i++){
        p_1 = p_neighbors[t_i];
        p_2 = Eigen::Vector3f::Zero();
        p_3 = Eigen::Vector3f::Zero();
        float min_fabs = 2 * (ros_config_->interp_hor_thr_ + ros_config_->interp_ver_thr_);
        float x = p.vec_(0) - p_1(0);
        float y = p.vec_(1) - p_1(1);
        float alpha{0}, beta{0};
        for(size_t i = t_i+1; i < neibor_size-1; i++){
            float single_fabs = std::fabs(p_neighbors[i](0) - p.vec_(0)) + std::fabs(p_neighbors[i](1) - p.vec_(1));
            if(single_fabs < min_fabs){
                p_2 = p_neighbors[i];
                
                for(size_t ii = i+1; ii < neibor_size; ii++){
                    float cur_fabs = std::fabs(p_neighbors[i](0) - p.vec_(0)) + std::fabs(p_neighbors[i](1) - p.vec_(1))
                            + std::fabs(p_neighbors[ii](0) - p.vec_(0)) + std::fabs(p_neighbors[ii](1) - p.vec_(1));
                    if(cur_fabs < min_fabs){
                        float x1 = p_neighbors[i](0) - p_1(0);
                        float x2 = p_neighbors[ii](0) - p_1(0);
                        float y1 = p_neighbors[i](1) - p_1(1);
                        float y2 = p_neighbors[ii](1) - p_1(1);
                        float lower = x1*y2 - x2*y1;

                        if(std::fabs(lower) > 10E-5){
                            alpha = (x*y2 - y*x2) / lower;
                            beta = -(x*y1 - y*x1) / lower;
                            if(alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && (alpha + beta) > 0 && (alpha + beta) < 1){
                                p_3 = p_neighbors[ii];
                                min_fabs = cur_fabs; 
                            }
                        }
                    }
                }
            }
        }

        if(p_2(2) < 10E-5 || p_3(2) < 10E-5) continue;

        float depth_cal = (1-alpha-beta)*p_1(2) + alpha*p_2(2) + beta*p_3(2);

        p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_) = -2;
        
        return depth_cal;
    }

    if(static_num > 0 && static_cast<int>(neibor_size) < all_num/2){
        p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_) = -2;
        return -2;
    }
    else{
        p.last_depth_interps_.at(map_index - depth_map_list_.front()->map_index_) = -2;
        return -2;
    }
}

bool DynObjFilter::checkMapConsistencyCase_1(PointSoph& p, const DepthMap& map, bool is_interp){
    float hor_half = std::max(ros_config_->map_cons_hor_thr1_, //0.05
            ros_config_->map_cons_hor_dis1_ / std::max(p.vec_(2), ros_config_->blind_dis_)); //0.3 0.3

    float ver_half = std::max(ros_config_->map_cons_ver_thr1_, 
            ros_config_->map_cons_ver_dis1_ / std::max(p.vec_(2), ros_config_->blind_dis_));

    float cur_map_cons_depth_thr1 = std::max(cutoff_value_, 
            ros_config_->k_depth_max_thr1_ * (p.vec_(2) - ros_config_->d_depth_max_thr1_))
                + ros_config_->map_cons_depth_thr1_;

    float cur_map_cons_min_thr1 = std::max(cutoff_value_, 
            ros_config_->k_depth_min_thr1_ * (p.vec_(2) - ros_config_->d_depth_min_thr1_))
                + ros_config_->enter_min_thr1_;

    float cur_map_cons_max_thr1 = std::max(cutoff_value_,
            ros_config_->k_depth_max_thr1_ * (p.vec_(2) - ros_config_->d_depth_max_thr1_))
                + ros_config_->enter_max_thr1_;

    if(ros_config_->dataset_ == 0 && p.is_distort_){
        cur_map_cons_depth_thr1 *= enlarge_distort_;
        cur_map_cons_min_thr1 *= enlarge_distort_;
        cur_map_cons_max_thr1 *= enlarge_distort_;
    }

    if(std::fabs(p.vec_(1)) < ros_config_->enlarge_z_thr1_/57.3){
        hor_half *= ros_config_->enlarge_angle_;
        ver_half *= ros_config_->enlarge_angle_;
        cur_map_cons_depth_thr1 *= ros_config_->enlarge_depth_;
    }

    int cur_map_cons_hor_num1 = std::ceil(hor_half / ros_config_->hor_resolution_max_);
    int cur_map_cons_ver_num1 = std::ceil(ver_half / ros_config_->ver_resolution_max_);

    for(int index_hor = -cur_map_cons_hor_num1; index_hor <= cur_map_cons_hor_num1; index_hor++){
        for(int index_ver = -cur_map_cons_ver_num1; index_ver <= cur_map_cons_ver_num1; index_ver++){

            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);

            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;
            
            if(map.min_depth_static_[pos_new] > p.vec_(2) + cur_map_cons_max_thr1
                || map.max_depth_static_[pos_new] < p.vec_(2) - cur_map_cons_min_thr1) continue; //小于最小深度，大于最大深度，跳过
            
            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new]; //深度图中一整行

            for(size_t j{0}; j < points_in_pixel.size(); j++){
                
                const PointSoph* point = points_in_pixel[j];
                
                if(point->dyn_flag_ == DynObjFlag::STATIC
                    && (std::fabs(p.vec_(2) - point->vec_(2)) < cur_map_cons_depth_thr1
                        || (p.vec_(2)-point->vec_(2) > cur_map_cons_depth_thr1 
                                && p.vec_(2) - point->vec_(2) < cur_map_cons_min_thr1))
                    && std::fabs(p.vec_(0) - point->vec_(0)) < hor_half
                    && std::fabs(p.vec_(1) - point->vec_(1)) < ver_half){
                        return true;
                }
            }
        }
    }

    if(is_interp && (p.local_(0) < ros_config_->self_x_b_ || p.local_(0) > ros_config_->self_x_f_ 
            || p.local_(1) > ros_config_->self_y_l_ || p.local_(1) < ros_config_->self_y_r_)){
        
        float depthmap_static = depthMapInterpolationStatic(p, map.map_index_, map.depth_map_);
        float cur_interp = ros_config_->interp_thr1_;

        if(p.vec_(2) > ros_config_->interp_start_depth1_){
            cur_interp += (ros_config_->interp_kp1_ * (p.vec_(2) - ros_config_->interp_start_depth1_) + ros_config_->interp_kd1_);
        }

        if(ros_config_->dataset_ == 0){
            if(std::fabs(depthmap_static + 1) < 10E-5 || std::fabs(depthmap_static + 2) < 10E-5)
                return false;
            else{
                if(std::fabs(depthmap_static - p.vec_(2)) < cur_interp)
                    return true;
            }
        }
        else{
            if(std::fabs(depthmap_static + 1) < 10E-5 || std::fabs(depthmap_static + 2) < 10E-5)
                return false;
            else{
                if(std::fabs(depthmap_static - p.vec_(2)) < cur_interp)
                    return true;
            }
        }
    }

    return false;
}

bool DynObjFilter::falseRejestionCase_1(PointSoph& p, const DepthMap& map){
    return checkMapConsistencyCase_1(p, map, ros_config_->case1_interp_en_);
}

bool DynObjFilter::isCase_1(PointSoph& p){
    // std::cout << "isCase_1 ";
    int depth_map_size = depth_map_list_.size();   //一般为5
    int occulude_map = depth_map_size; //最大遮挡量

    // std::cout << "a ";
    
    for(int i{ depth_map_size-1 }; i >= 0; i--){
        // std::cout << "b ";
        sphericalProjection(p, depth_map_list_[i]->map_index_, depth_map_list_[i]->project_R_, depth_map_list_[i]->project_T_, p);
        // std::cout << "c ";

        //越界，跳过
        if(std::fabs(p.hor_ind_) > MAX_1D || std::fabs(p.ver_ind_) > MAX_1D_HALF || p.vec_(2) < 0.0f || p.position_ < 0 || p.position_ >= MAX_2D_N){
            p.dyn_flag_ = DynObjFlag::INVALID;
            continue;
        }

        // if(checkMapConsistencyCase_1(p, *depth_map_list_[i], ros_config_->case1_interp_en_)) {}

        // std::cout << "d ";
        if(enterCase_1(p, *depth_map_list_[i])){
            if(falseRejestionCase_1(p, *depth_map_list_[i])) occulude_map -= 1;
        }
        else occulude_map -= 1;

        // std::cout << "e ";

        if(occulude_map < ros_config_->occluded_map_thr1_){
            // num_11++;
            return false;
        } 

        // std::cout << "f ";

        if(occulude_map - i >= ros_config_->occluded_map_thr1_){ //>= 3
            // num_12++;
            return true;
        } 
    }

    // std::cout << "g ";

    // num_10 = depth_map_size;
    //不可能发生
    if(occulude_map >= ros_config_->occluded_map_thr1_){
        // num_13++;
        return true;
    } 

    // std::cout << "h ";
    // num_14++;
    return false;
}

bool DynObjFilter::enterCase_2(const PointSoph& p, const DepthMap& map){
    if(p.dyn_flag_ != DynObjFlag::STATIC) return false;

    float max_depth{0.0};
    float depth_thr2_final = std::max(cutoff_value_,
             ros_config_->k_depth_max_thr2_ * (p.vec_(2) - ros_config_->d_depth_max_thr2_))
            + ros_config_->occ_depth_thr2_;
    
    if(map.depth_map_[p.position_].size() > 0){
        const PointSoph* max_point = map.depth_map_[p.position_][map.max_depth_index_all_[p.position_]];
        max_depth = max_point->vec_(2);
        float delta_t = p.time_ - max_point->time_;
        depth_thr2_final = std::min(depth_thr2_final, ros_config_->v_min_thr2_ * delta_t);
    }

    if(p.vec_(2) > max_depth + depth_thr2_final){
        case2_num_++;
        return true;
    }
    return false;
}

float DynObjFilter::depthMapInterpolationAll(PointSoph& p, const std::vector<std::vector<PointSoph*>>& depth_map){

    // std::cout << "depthMapInterpolationAll\n";

    Eigen::Vector3f p_1 = Eigen::Vector3f::Zero();
    Eigen::Vector3f p_2 = Eigen::Vector3f::Zero();
    Eigen::Vector3f p_3 = Eigen::Vector3f::Zero();
    std::vector<Eigen::Vector3f> p_neighbors;
    int all_num{0};

    for(int index_hor = -interp_hor_num_; index_hor <= interp_hor_num_; index_hor++){
        for(int index_ver = -interp_ver_num_; index_ver <= interp_ver_num_; index_ver++){
            
            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            const std::vector<PointSoph*> & points_in_pixel = depth_map[pos_new];

            for(size_t j{0}; j < points_in_pixel.size(); j++){

                const PointSoph* point = points_in_pixel[j];
                
                if(std::fabs(point->time_ - p.time_) < ros_config_->frame_dur_) continue;

                float hor_minus = point->vec_(0) - p.vec_(0);
                float ver_minus = point->vec_(1) - p.vec_(1);

                if(std::fabs(hor_minus) < ros_config_->interp_hor_thr_ && std::fabs(ver_minus) < ros_config_->interp_ver_thr_){
                    all_num++;
                    p_neighbors.push_back(point->vec_);
                    if(p_1(2)<0.000001 || std::fabs(hor_minus) + std::fabs(ver_minus) < std::fabs(p_1(0) - p.vec_(0)) + std::fabs(p_1(1) - p.vec_(1))){
                        p_1 = point->vec_;
                    }  
                }
            }
        }
    }

    size_t neibor_size = p_neighbors.size();

    if(p_1(2) < 10E-5 || neibor_size < 3) return -1;

    for(size_t t_i{0}; t_i < neibor_size-2; t_i++){

        p_1 = p_neighbors[t_i];
        p_2 = Eigen::Vector3f::Zero();
        p_3 = Eigen::Vector3f::Zero();

        float min_fabs = 2*(ros_config_->interp_hor_thr_ + ros_config_->interp_ver_thr_);
        float x = p.vec_(0) - p_1(0);
        float y = p.vec_(1) - p_1(1);
        float alpha {0}, beta {0};

        for(size_t i = t_i+1; i < neibor_size-1; i++){

            if(std::fabs(p_neighbors[i](0) - p.vec_(0)) + std::fabs(p_neighbors[i](1) - p.vec_(1)) < min_fabs){
                p_2 = p_neighbors[i];
                float single_fabs = std::fabs(p_neighbors[i](0)-p.vec_(0)) + std::fabs(p_neighbors[i](1)-p.vec_(1));
                
                if(single_fabs >= min_fabs) continue;
                
                for(size_t ii = i+1; ii < neibor_size; ii++){
                    float cur_fabs = std::fabs(p_neighbors[i](0) - p.vec_(0)) + std::fabs(p_neighbors[i](1) - p.vec_(1))
                            + std::fabs(p_neighbors[ii](0) - p.vec_(0)) + std::fabs(p_neighbors[ii](1) - p.vec_(1));

                    if(cur_fabs < min_fabs){

                        float x1 = p_neighbors[i](0) - p_1(0);
                        float x2 = p_neighbors[ii](0) - p_1(0);
                        float y1 = p_neighbors[i](1) - p_1(1);
                        float y2 = p_neighbors[ii](1) - p_1(1);
                        float lower = x1*y2 - x2*y1;

                        if(std::fabs(lower) > 10E-5){

                            alpha = (x*y2-y*x2)/lower;
                            beta = -(x*y1-y*x1)/lower;

                            if(alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && (alpha + beta) > 0 && (alpha + beta) < 1){

                                p_3 = p_neighbors[ii];
                                min_fabs = cur_fabs; 
                            }                        
                        }
                    }
                }
            }
        }

        if(p_2(2) < 10E-5 || p_3(2) < 10E-5) continue;

        return (1-alpha-beta)*p_1(2) + alpha*p_2(2) + beta*p_3(2);
    }
    return -2;
}

bool DynObjFilter::checkMapConsistencyCase_2(PointSoph& p, const DepthMap& map, bool is_interp){

    // std::cout << "checkMapConsistencyCase_2\n";

    float cur_depth = std::max(cutoff_value_, 
            ros_config_->k_depth_max_thr2_ * (p.vec_(2) - ros_config_->d_depth_max_thr2_))
            + ros_config_->map_cons_depth_thr2_;

    for(int index_hor = -map_cons_hor_num2_; index_hor <= map_cons_hor_num2_; index_hor++){
        for(int index_ver = -map_cons_ver_num2_; index_ver <= map_cons_ver_num2_; index_ver++){

            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);

            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            if(map.min_depth_all_[pos_new] < p.vec_(2) - cur_depth 
                    && map.max_depth_all_[pos_new] > p.vec_(2) + cur_depth){
                continue;
            } 

            const std::vector<PointSoph*>& point_in_pixel = map.depth_map_[pos_new];
            
            for(size_t j{0}; j < point_in_pixel.size(); j++){

                const PointSoph* point = point_in_pixel[j];

                if(point->dyn_flag_ == DynObjFlag::STATIC
                        && std::fabs(p.time_ - point->time_) > ros_config_->frame_dur_
                        && std::fabs(p.vec_(2) - point->vec_(2)) < cur_depth
                        && std::fabs(p.vec_(1) - point->vec_(1) < ros_config_->map_cons_ver_thr2_
                        && std::fabs(p.vec_(0) - point->vec_(0)) < ros_config_->map_cons_hor_thr2_)){
                    return true;
                }
            }
        }
    }

    if(is_interp && (p.local_(0) < ros_config_->self_x_b_
                        || p.local_(0) > ros_config_->self_x_f_
                        || p.local_(1) > ros_config_->self_y_l_
                        || p.local_(1) < ros_config_->self_y_r_)){
        
        float cur_interp = ros_config_->interp_thr2_ * (depth_map_list_.back()->map_index_ - map.map_index_ + 1);
        float depth_all = depthMapInterpolationAll(p, map.depth_map_);

        return std::fabs(p.vec_(2) - depth_all) < cur_interp; 
    }
    return false;
}

bool DynObjFilter::isOccludedCase_2(const PointSoph& p, const PointSoph& p_occ){
    
    // std::cout << "isOccludedCase_2\n";

    if((ros_config_->dataset_ == 0 && p_occ.is_distort_)
            || (ros_config_->dataset_ == 0 && p.is_distort_)
            || p_occ.dyn_flag_ == DynObjFlag::INVALID) return false;
    
    if((p.local_(0) > ros_config_->self_x_b_ && p.local_(0) < ros_config_->self_x_f_ 
            && p.local_(1) < ros_config_->self_y_l_ && p.local_(1) > ros_config_->self_y_r_)
        || (p_occ.local_(0) > ros_config_->self_x_b_ && p_occ.local_(0) < ros_config_->self_x_f_
            && p_occ.local_(1) < ros_config_->self_y_l_ && p_occ.local_(1) > ros_config_->self_y_r_)) return false;
    
    float delta_t = p.time_ - p_occ.time_;
    float cur_occ_hor = ros_config_->occ_hor_thr2_;
    float cur_occ_ver = ros_config_->occ_ver_thr2_;

    if(delta_t > 0){
        float depth_thr2_final = std::min(ros_config_->v_min_thr2_ * delta_t, 
                std::max(cutoff_value_, 
                        ros_config_->k_depth_max_thr2_ * (p.vec_(2) - ros_config_->d_depth_max_thr2_) + ros_config_->occ_depth_thr2_));
        
        if(p.vec_(2) > p_occ.vec_(2) + depth_thr2_final
                && std::fabs(p.vec_(0) - p_occ.vec_(0) < cur_occ_hor)
                && std::fabs(p.vec_(1) - p_occ.vec_(1) < cur_occ_ver)) return true;
    }
    return false;
}

bool DynObjFilter::checkDepthConsistencyCase_2(const PointSoph& p, const DepthMap& map){

    // std::cout << "checkDepthConsistencyCase_2 ";

    float all_minus{0};
    int num{0}, smaller_num{0}, all_num{0}, greater_num{0};

    //for get 4 num
    for(int index_hor = -depth_cons_hor_num2_; index_hor <= depth_cons_hor_num2_; index_hor++){
        for(int index_ver = -depth_cons_ver_num2_; index_ver <=depth_cons_ver_num2_; index_ver++){

            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new];

            for(size_t j{0}; j < points_in_pixel.size(); j++){

                const PointSoph* point = points_in_pixel[j];
                
                if(std::fabs(point->time_ - p.time_) < ros_config_->frame_dur_
                        && std::fabs(point->vec_(0) - p.vec_(0) < ros_config_->depth_cons_hor_thr2_)
                        && std::fabs(point->vec_(1) - p.vec_(1) < ros_config_->depth_cons_ver_thr2_)){

                    all_num++;

                    if(point->dyn_flag_ == DynObjFlag::STATIC){
                        float cur_minus = p.vec_(2) - point->vec_(2);

                        if(std::fabs(cur_minus) < ros_config_->depth_cons_depth_max_thr2_){
                            num++;
                            all_minus += std::fabs(point->vec_(2) - p.vec_(2));
                        }
                        else if(all_minus >0) smaller_num++;
                        else greater_num++;
                    }
                }
            }
        }
    }

    if(all_num > 0){
        if(num > 1){
            float cur_depth_thr = std::max(ros_config_->depth_cons_depth_thr2_, 
                    ros_config_->k_depth2_ * p.vec_(2));
            if(all_minus / (num-1) > cur_depth_thr){
                // std::cout << "a\n";
                return false;
            } 
        }
        // std::cout << "b " << greater_num << " " << smaller_num << "\n";
        return (greater_num == 0 || smaller_num == 0);
    }
    else{
        // std::cout << "c\n";
        return false;
    } 
}

bool DynObjFilter::searchPointOculudingPCase_2(PointSoph& p, const DepthMap& map){

    // std::cout << "searchPointOculudingPCase_2\n";

    for(int index_hor = -occ_hor_num2_; index_hor <= occ_hor_num2_; index_hor++){
        for(int index_ver = -occ_ver_num2_; index_ver <= occ_ver_num2_; index_ver++){

            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            if(map.min_depth_all_[pos_new] > p.vec_(2)) continue;

            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new];
            
            for(size_t j{0}; j < points_in_pixel.size(); j++){
                const PointSoph* p_cond = points_in_pixel[j];
                
                if(isOccludedCase_2(p, *p_cond) && checkDepthConsistencyCase_2(*p_cond, map)){
                    p.occu_index_[0] = map.map_index_;
                    p.occu_index_[1] = pos_new;
                    p.occu_index_[2] = j;
                    p.occ_vec_ = p.vec_;
                    return true;
                }
            }
        }
    }
    return false;
}

bool DynObjFilter::checkVelCase_2(float v1, float v2, double delta_t){
    return std::fabs(v1 - v2) < delta_t * ros_config_->acc_thr2_;
}

bool DynObjFilter::isCase_2(PointSoph& p){
    if(ros_config_->dataset_ == 0 && p.is_distort_) return false;

    size_t first_index = depth_map_list_.size();
    if(first_index == 0) return false;
    first_index -= 1;

    PointSoph p_spherical = p;
    sphericalProjection(p, depth_map_list_[first_index]->map_index_, 
            depth_map_list_[first_index]->project_R_, depth_map_list_[first_index]->project_T_, p_spherical);

    if(std::fabs(p_spherical.hor_ind_) >= MAX_1D || std::fabs(p_spherical.ver_ind_) >= MAX_1D_HALF || p_spherical.vec_(2) < 0.0f
            || p_spherical.position_ < 0 || p_spherical.position_ >= MAX_2D_N){

        p.dyn_flag_ = DynObjFlag::INVALID;
        return false;
    }

    int cur_occ_times{0};

    if(enterCase_2(p_spherical, *depth_map_list_[first_index])){

        if(!checkMapConsistencyCase_2(p_spherical, *depth_map_list_[first_index], ros_config_->case2_interp_en_)){
            double ti{0};
            float vi{0};
            bool is_map_cons{true};

            for(int index_hor = -occ_hor_num2_; index_hor <= occ_hor_num2_; index_hor++){
                for(int index_ver = -occ_ver_num2_; index_ver <= occ_ver_num2_; index_ver++){

                    int pos_new = MAX_1D_HALF * ((p_spherical.hor_ind_ + index_hor) % MAX_1D) + ((p_spherical.ver_ind_ + index_ver) % MAX_1D_HALF);
                    
                    if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

                    if(depth_map_list_[first_index]->min_depth_all_[pos_new] > p_spherical.vec_(2)) continue;

                    const std::vector<PointSoph*>& points_in_pixel = depth_map_list_[first_index]->depth_map_[pos_new];

                    for(size_t k{0}; k < points_in_pixel.size() && is_map_cons; k++){

                        const PointSoph* p_occ = points_in_pixel[k];

                        if(isOccludedCase_2(p_spherical, *p_occ) && checkDepthConsistencyCase_2(*p_occ, *depth_map_list_[first_index])){

                            // std::cout << "isCase_2 1\n";

                            cur_occ_times = 1;

                            if(cur_occ_times >= ros_config_->occluded_times_thr2_) break;

                            ti = (p_occ->time_ + p.time_) / 2;
                            vi = (p_spherical.vec_(2) - p_occ->vec_(2)) / (p.time_ - p_occ->time_);
                            p.occu_index_[0] = depth_map_list_[first_index]->map_index_;
                            p.occu_index_[1] = pos_new;
                            p.occu_index_[2] = static_cast<int>(k);
                            p.occ_vec_ = p_spherical.vec_;
                            p.occu_times_ = cur_occ_times;

                            PointSoph p1 = *points_in_pixel[k];
                            int i = static_cast<int>(depth_map_list_.size());
                            i -= 2;
                            Eigen::Vector3d t2;
                            t2.setZero();

                            // std::cout << "isCase_2 2 a\n";


                            // int num_test{0};

                            while (i >= 0){
                                // std::cout << "i: " << i << " " << num_test++ << " ";
                                if(p1.occu_index_[0] == -1 || p1.occu_index_[0] < depth_map_list_.front()->map_index_){
                                    sphericalProjection(p1, depth_map_list_[i]->map_index_, depth_map_list_[i]->project_R_, 
                                            depth_map_list_[i]->project_T_, p1);
                                    
                                    if(searchPointOculudingPCase_2(p1, *depth_map_list_[i])) p1.occ_vec_ = p1.vec_;
                                    else break;
                                }
                                
                                i = p1.occu_index_[0] - depth_map_list_.front()->map_index_; //处理不当,与下方i--会形成死循环
                                PointSoph* p2 = depth_map_list_[i]->depth_map_[p1.occu_index_[1]][p1.occu_index_[2]];

                                sphericalProjection(p, depth_map_list_[i]->map_index_, depth_map_list_[i]->project_R_, 
                                        depth_map_list_[i]->project_T_, p);

                                if(checkMapConsistencyCase_2(p, *depth_map_list_[i], ros_config_->case2_interp_en_)){
                                    is_map_cons = false;
                                    break;
                                }

                                float vc = (p1.occ_vec_(2) - p2->vec_(2)) / (p1.time_ - p2->time_);
                                double tc = (p1.time_ + p2->time_) /2;

                                if(isOccludedCase_2(p, *p2) 
                                        && checkDepthConsistencyCase_2(*p2, *depth_map_list_[i])
                                        && checkVelCase_2(vi, vc, ti-tc)){
                                    cur_occ_times += 1;
                                    
                                    if(cur_occ_times >= ros_config_->occluded_times_thr2_){
                                        p.occu_times_ = cur_occ_times;
                                        return true;
                                    }

                                    t2 = p2->glob_;
                                    p1 = *p2;
                                    vi = vc;
                                    ti = tc;
                                }
                                else break;
                                i--;
                            }
                            // std::cout << "isCase_2 2 b\n";
                        }
                        // std::cout << "isCase_2 3\n";
                        if(cur_occ_times >= ros_config_->occluded_times_thr2_) break;
                    }
                    // std::cout << "isCase_2 4\n";
                    if(cur_occ_times >= ros_config_->occluded_times_thr2_) break;
                }
                if(cur_occ_times >= ros_config_->occluded_times_thr2_) break;
            }
        }
    }

    if(cur_occ_times >= ros_config_->occluded_times_thr2_){
        p.occu_times_ = cur_occ_times;
        return true;
    }
    return false;
}

bool DynObjFilter::enterCase_3(const PointSoph& p, const DepthMap& map){
    if(p.dyn_flag_ != DynObjFlag::STATIC) return false;

    float min_depth{0};
    float depth_thr3_final = std::max(cutoff_value_, 
            ros_config_->k_depth_max_thr3_ * (p.vec_(2) - ros_config_->d_depth_max_thr3_))
                    + ros_config_->occ_depth_thr3_;
    
    if(map.depth_map_[p.position_].size() > 0){
        const PointSoph* min_point = map.depth_map_[p.position_][map.min_depth_index_all_[p.position_]];
        min_depth = min_point->vec_(2);
        float delta_t = p.time_ - min_point->time_;
        depth_thr3_final = std::min(depth_thr3_final, ros_config_->v_min_thr3_ * delta_t);
    }

    if(ros_config_->dataset_ == 0 && p.is_distort_){
        depth_thr3_final *= enlarge_distort_;
    }

    if(p.vec_(2) < min_depth - depth_thr3_final){
        case3_num_++;
        return true;
    }
    return false;
}

bool DynObjFilter::checkMapConsistencyCase_3(PointSoph& p, const DepthMap& map, bool is_interp){
    float cur_v_min = ros_config_->v_min_thr3_;
    float cur_hor = ros_config_->map_cons_hor_thr3_;
    float cur_ver = ros_config_->map_cons_ver_thr3_;
    float cur_depth = std::max(cutoff_value_, 
            ros_config_->k_depth_max_thr3_ * (p.vec_(2) - ros_config_->d_depth_max_thr3_))
                    + ros_config_->map_cons_depth_thr3_;

    if(ros_config_->dataset_ ==0 && p.is_distort_)
        cur_v_min *= enlarge_distort_;

    for(int index_hor = -map_cons_hor_num3_; index_hor <= map_cons_hor_num3_; index_hor++){
        for(int index_ver = -map_cons_ver_num3_; index_ver <= map_cons_ver_num3_; index_ver++){
            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            if(map.min_depth_all_[pos_new] < p.vec_(2) - cur_depth 
                    && map.max_depth_all_[pos_new] > p.vec_(2) + cur_depth){
                continue;
            }

            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new];

            for(size_t j{0}; j < points_in_pixel.size(); j++){
                const PointSoph* point = points_in_pixel[j];

                if(point->dyn_flag_ == DynObjFlag::STATIC
                        && std::fabs(p.time_ - point->time_) > ros_config_->frame_dur_
                        && point->vec_(2) - p.vec_(2) < cur_depth
                        && std::fabs(p.vec_(1) - point->vec_(1)) < cur_ver
                        && std::fabs(p.vec_(0) - point->vec_(0)) < cur_hor){
                    return true;
                }
            }
        }
    }

    if(is_interp 
            && (p.local_(0) < ros_config_->self_x_b_
                    || p.local_(0) > ros_config_->self_x_f_
                    || p.local_(1) > ros_config_->self_y_l_
                    || p.local_(1) < ros_config_->self_y_r_)){

        float cur_interp = ros_config_->interp_thr3_ * (depth_map_list_.back()->map_index_ - map.map_index_ + 1);
        float depth_all = depthMapInterpolationAll(p, map.depth_map_);

        return std::fabs(p.vec_(2) - depth_all) < cur_interp;
    }
    return false;
}

bool DynObjFilter::isOccludedCase_3(const PointSoph& p, const PointSoph& p_occ){
    if((ros_config_->dataset_ == 0 && p_occ.is_distort_)
            || (ros_config_->dataset_ == 0 && p.is_distort_)
            || p_occ.dyn_flag_ == DynObjFlag::INVALID) return false;
    
    if((p.local_(0) > ros_config_->self_x_b_ && p.local_(0) < ros_config_->self_x_f_ 
                && p.local_(1) < ros_config_->self_y_l_ && p.local_(1) > ros_config_->self_y_r_)
        || (p_occ.local_(0) > ros_config_->self_x_b_ && p_occ.local_(0) < ros_config_->self_x_f_ 
                && p_occ.local_(1) < ros_config_->self_y_l_ && p_occ.local_(1) > ros_config_->self_y_r_))
        return false;
    
    float delta_t = p.time_ - p_occ.time_;

    if(delta_t > 0){
        float depth_thr3_final = std::min(ros_config_->v_min_thr3_ * delta_t, 
                std::max(cutoff_value_, 
                        ros_config_->k_depth_max_thr3_ * (p.vec_(2) - ros_config_->d_depth_max_thr3_))
                 + ros_config_->map_cons_depth_thr3_);

        if(ros_config_->dataset_ == 0 && p.is_distort_)
            depth_thr3_final *= enlarge_distort_;

        if(p.occ_vec_(2) > p.vec_(2) + depth_thr3_final
                && std::fabs(p.vec_(1) - p_occ.vec_(0)) < ros_config_->occ_hor_thr3_
                && std::fabs(p.vec_(0) - p_occ.vec_(0)) < ros_config_->occ_ver_thr3_){
            return true;
        }
    }
    return false;
}

bool DynObjFilter::checkDepthConsistencyCase_3(const PointSoph& p, const DepthMap& map){
    
    float all_minus{0};
    int num{0}, smaller_num{0}, all_num{0}, greater_num{0};
    
    for(int index_hor = -depth_cons_hor_num3_; index_hor <= depth_cons_hor_num3_; index_hor++){
        for(int index_ver = -depth_cons_ver_num3_; index_ver <= depth_cons_ver_num3_; index_ver++){
            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);

            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new];

            for(size_t j{0}; j < points_in_pixel.size(); j++){
                const PointSoph* point = points_in_pixel[j];

                if(std::fabs(point->time_ - p.time_) < ros_config_->frame_dur_
                        && std::fabs(point->vec_(0) - p.vec_(0) < ros_config_->depth_cons_hor_thr3_
                        && std::fabs(point->vec_(1) - p.vec_(1) < ros_config_->depth_cons_ver_thr3_))){
                    all_num++;

                    if(point->dyn_flag_ == DynObjFlag::STATIC){
                        float cur_minus = p.vec_(2) - point->vec_(2);

                        if(std::fabs(cur_minus) < ros_config_->depth_cons_depth_max_thr3_){
                            num++;
                            all_minus += std::fabs(point->vec_(2) - p.vec_(2));
                        }
                        else if(cur_minus > 0) smaller_num++;
                        else greater_num++;
                    }
                }
            }
        }
    }

    if(all_num > 0){
        if(num > 1){
            float cur_depth_thr 
                = std::max(ros_config_->depth_cons_depth_thr3_, ros_config_->k_depth3_ * p.vec_(2));
            
            if(all_minus / (num-1) > cur_depth_thr) return false;

            return greater_num == 0 || smaller_num == 0;
        }
    }
    return false;
}

bool DynObjFilter::searchPointOculudingPCase_3(PointSoph& p, const DepthMap& map){
    for(int index_hor = -occ_hor_num3_; index_hor <= occ_hor_num3_; index_hor++){
        for(int index_ver = -occ_ver_num3_; index_ver <= occ_ver_num3_; index_ver++){

            int pos_new = MAX_1D_HALF * ((p.hor_ind_ + index_hor) % MAX_1D) + ((p.ver_ind_ + index_ver) % MAX_1D_HALF);
            if(pos_new < 0 || pos_new >= MAX_2D_N) continue;

            if(map.min_depth_all_[pos_new] > p.vec_(2)) continue;

            const std::vector<PointSoph*>& points_in_pixel = map.depth_map_[pos_new];
            for(size_t j{0}; j < points_in_pixel.size(); j++){

                const PointSoph* p_cond = points_in_pixel[j];
            
                if(isOccludedCase_3(p, *p_cond) && checkDepthConsistencyCase_3(*p_cond, map)){
                    p.is_occu_index_[0] = map.map_index_;
                    p.is_occu_index_[1] = pos_new;
                    p.is_occu_index_[2] = j;
                    p.occ_vec_ = p.vec_;
                    return true;
                }
            }
        }
    }
    return false;
}

bool DynObjFilter::checkVelCase_3(float v1, float v2, double delta_t){
    return std::fabs(v1 - v2) < ros_config_->acc_thr3_ * delta_t;
}

bool DynObjFilter::isCase_3(PointSoph& p){
    if(ros_config_->dataset_ == 0 && p.is_distort_) return false;
    
    size_t first_index = depth_map_list_.size();
    if(first_index == 0) return false;
    first_index -= 1;
    
    PointSoph p_spherical = p;
    sphericalProjection(p, depth_map_list_[first_index]->map_index_, depth_map_list_[first_index]->project_R_, 
            depth_map_list_[first_index]->project_T_, p_spherical);
    
    if(std::fabs(p_spherical.hor_ind_) >= MAX_1D || std::fabs(p_spherical.ver_ind_) >= MAX_1D_HALF
            || p_spherical.vec_(2) < 0.0f || p_spherical.position_ < 0 || p_spherical.position_ >=  MAX_2D_N){
        p.dyn_flag_ = DynObjFlag::INVALID;
        return false;            
    }

    int cur_occ_times{0};

    if(enterCase_3(p_spherical, *depth_map_list_[first_index])){
        if(!checkMapConsistencyCase_3(p_spherical, *depth_map_list_[first_index], ros_config_->case3_interp_en_)){
            
            double ti{0};
            float vi{0};
            // float min_hor = ros_config_->occ_hor_thr3_, min_ver = ros_config_->occ_ver_thr3_;
            bool is_map_cons{true};

            for(int index_hor = -occ_hor_num3_; index_hor <= occ_hor_num3_; index_hor++){
                for(int index_ver = -occ_ver_num3_; index_ver <= occ_ver_num3_; index_ver++){
                    
                    int pos_new = MAX_1D_HALF * ((p_spherical.hor_ind_ + index_hor) % MAX_1D) + ((p_spherical.ver_ind_ + index_ver) % MAX_1D_HALF);
                    if(pos_new < 0 || pos_new <= MAX_2D_N) continue;

                    if(depth_map_list_[first_index]->max_depth_all_[pos_new] < p_spherical.vec_(2)) continue;

                    const std::vector<PointSoph*>& points_in_pixel = depth_map_list_[first_index]->depth_map_[pos_new];

                    for(size_t k{0}; k < points_in_pixel.size() && is_map_cons; k++){

                        const PointSoph* p_occ = points_in_pixel[k];

                        if(isOccludedCase_3(p_spherical, *p_occ) && checkDepthConsistencyCase_3(*p_occ, *depth_map_list_[first_index])){
                            
                            cur_occ_times = 1;
                            ti = (p_occ->time_ + p.time_) / 2;
                            vi = (p_occ->vec_(2) - p_spherical.vec_(2)) / (p.time_ - p_occ->time_);
                            p.is_occu_index_[0] = depth_map_list_[first_index]->map_index_;
                            p.is_occu_index_[1] = pos_new;
                            p.is_occu_index_[2] = k;
                            p.is_occ_vec_ = p_spherical.vec_;
                            p.is_occu_times_ = cur_occ_times;

                            PointSoph p1 = *points_in_pixel[k];
                            int i = static_cast<int>(depth_map_list_.size()) - 2;

                            while(i >= 0){
                                if(p1.is_occu_index_[0] == -1 || p1.is_occu_index_[0] < depth_map_list_.front()->map_index_){

                                    sphericalProjection(p1, depth_map_list_[i]->map_index_, depth_map_list_[i]->project_R_, depth_map_list_[i]->project_T_, p);
                                    
                                    if(searchPointOculudingPCase_3(p1, *depth_map_list_[i])){
                                        p1.is_occ_vec_ = p1.vec_;
                                    }
                                    else break;
                                }

                                i = p1.is_occu_index_[0] - depth_map_list_.front()->map_index_;

                                PointSoph* p2 = depth_map_list_[i]->depth_map_[p1.is_occu_index_[1]] [p1.is_occu_index_[2]];

                                sphericalProjection(p, depth_map_list_[i]->map_index_, depth_map_list_[i]->project_R_, 
                                        depth_map_list_[i]->project_T_, p);
                                
                                if(checkMapConsistencyCase_3(p, *depth_map_list_[i], ros_config_->case3_interp_en_)){
                                    is_map_cons = false;
                                    break;
                                }

                                float vc = -(p1.is_occ_vec_(2) - p2->vec_(2)) / (p1.time_ - p2->time_);
                                float tc = (p1.time_ + p2->time_) / 2;

                                if(isOccludedCase_3(p, *p2)
                                        && checkDepthConsistencyCase_3(*p2, *depth_map_list_[i])
                                        && checkVelCase_3(vi, vc, ti-tc)){
                                    
                                    cur_occ_times++;

                                    if(cur_occ_times >= ros_config_->occluding_times_thr3_){
                                        p.is_occu_times_ = cur_occ_times;
                                        return true;    
                                    }

                                    p1 = *p2;
                                    vi = vc;
                                    ti = tc;
                                }
                                else break;

                                i--;
                            }
                        }
                        if(cur_occ_times >= ros_config_->occluding_times_thr3_) break;
                    }
                    if(cur_occ_times >= ros_config_->occluding_times_thr3_) break;
                }
                if(cur_occ_times >= ros_config_->occluding_times_thr3_) break;
            }
        }
    }

    if(cur_occ_times >= ros_config_->occluding_times_thr3_){
        p.is_occu_times_ = cur_occ_times;
        return true;
    }

    return false;
}

void DynObjFilter::bufferPoints(std::vector<PointSoph*>& points, std::vector<int>& index_vec){
    int tail = buffer_.tail_;
    buffer_.pushParallelPrepare(static_cast<int>(points.size()));
    std::for_each(std::execution::par, index_vec.begin(), index_vec.end(), [&](const int& i){
        buffer_.pushParallel(points[i], tail + i);
    });
}

void DynObjFilter::bufferDepthMap(double cur_time){
    int buffer_len = buffer_.size();

    //k = 6207 崩溃
    for(int k{0}; k < buffer_len; k++){
        // std::cout << " " << k << " 1- ";

        PointSoph* point = buffer_.front(); //取队列第一个元素

        // < 0.09
        if(cur_time - point->time_ < ros_config_->buffer_delay_ - ros_config_->frame_dur_/2.0){
            // std::cout << "a";
            break;
        }   //时间间隔不足,移除

        //下面的if和else if 用于更新和替换就的depthmap
        //依据是，最新的depthmap与point的时间差是否大于某个值
        //一旦更新，会替换depthmap的time为point的time，所以不会重复替换操作

        if(depth_map_list_.size() == 0){
            if(static_cast<int>(depth_map_list_.size()) < ros_config_->max_depth_map_num_){
                buffer_depth_map_index_++;
                DepthMap::Ptr map_points(new DepthMap(point->rot_, point->transl_, point->time_, buffer_depth_map_index_));
                depth_map_list_.push_back(map_points);
            }
            else{
                buffer_.pop();
                continue;
            }
        }
        // 与最新的depthmap的时间差 >= 0.09
        else if(point->time_ - depth_map_list_.back()->time_ >= ros_config_->depth_map_dur_ - ros_config_->frame_dur_/2.0){
            buffer_depth_map_index_++;

            // depth_map_list_满后删除头，在尾新添加
            if(static_cast<int>(depth_map_list_.size()) == ros_config_->max_depth_map_num_){
                //会遍历执行 depth_map_list_->depth_map_[i].clear();
                depth_map_list_.front()->reSet(point->rot_, point->transl_, point->time_, buffer_depth_map_index_);
                DepthMap::Ptr map_points = depth_map_list_.front();
                depth_map_list_.pop_front();
                depth_map_list_.push_back(map_points);
            }
            //未满则尾新添加
            else if(static_cast<int>(depth_map_list_.size()) < ros_config_->max_depth_map_num_){
                DepthMap::Ptr map_points(new DepthMap(point->rot_, point->transl_, point->time_, buffer_depth_map_index_));
                depth_map_list_.push_back(map_points);
            }
        }
        // std::cout << "2-";

        switch(point->dyn_flag_){
        // //     // if(depth_map_list_.back()->depth_map_.size() <= point->position_)
            case DynObjFlag::STATIC:
        //         if(depth_map_list_.back()->depth_map_.size() <= point->position_){
        //             std::cout << "break " << depth_map_list_.back()->depth_map_.size() << " <= " << point->position_ << "\n";
        //             break;
        //         }
                // std::cout << "break " << k << " | " << depth_map_list_.back()->depth_map_.size() << " | " << point->position_ << "";

                sphericalProjection(*point, depth_map_list_.back()->map_index_, depth_map_list_.back()->project_R_, 
                        depth_map_list_.back()->project_T_, *point);
                
                if(static_cast<int>(depth_map_list_.back()->depth_map_.size()) <= point->position_){ //自加， 避免报错
                    std::cout << "b";
                    break;
                } 

                if(static_cast<int>(depth_map_list_.back()->depth_map_[point->position_].size()) >= ros_config_->max_pixel_points_){  
                    // std::cout << "c";
                    break;  //源码是有；break
                } 

                depth_map_list_.back()->depth_map_[point->position_].push_back(point);
                
                if(point->vec_(2) > depth_map_list_.back()->max_depth_all_[point->position_]){
                    depth_map_list_.back()->max_depth_all_[point->position_] = point->vec_(2);
                    depth_map_list_.back()->max_depth_index_all_[point->position_] = 
                            depth_map_list_.back()->depth_map_[point->position_].size()-1;
                }

                if(point->vec_(2) < depth_map_list_.back()->min_depth_all_[point->position_]
                        || depth_map_list_.back()->min_depth_all_[point->position_] < 10E-5){
                    
                    depth_map_list_.back()->min_depth_all_[point->position_] = point->vec_(2);
                    depth_map_list_.back()->min_depth_index_all_[point->position_] = 
                            depth_map_list_.back()->depth_map_[point->position_].size()-1;
                }

                if(point->vec_(2) < depth_map_list_.back()->min_depth_static_[point->position_]
                        || depth_map_list_.back()->min_depth_static_[point->position_] < 10E-5){
                    
                    depth_map_list_.back()->min_depth_static_[point->position_] = point->vec_(2);
                }

                if(point->vec_(2) > depth_map_list_.back()->max_depth_static_[point->position_]){
                    depth_map_list_.back()->max_depth_static_[point->position_] = point->vec_(2);
                }

                break;

            case DynObjFlag::CASE1:
            case DynObjFlag::CASE2:
            case DynObjFlag::CASE3:
        //         if(depth_map_list_.back()->depth_map_.size() <= point->position_){
        //             std::cout << "break " << depth_map_list_.back()->depth_map_.size() << " <= " << point->position_ << "\n";
        //             break;
        //         } 

                std::cout << "break " << k << " | " << depth_map_list_.back()->depth_map_.size() << " | " << point->position_ << "";

                sphericalProjection(*point, depth_map_list_.back()->map_index_, depth_map_list_.back()->project_R_, 
                        depth_map_list_.back()->project_T_, *point);

                if(static_cast<int>(depth_map_list_.back()->depth_map_.size()) <= point->position_){  //自加， 避免报错
                    break;
                } 
                
                if(static_cast<int>(depth_map_list_.back()->depth_map_[point->position_].size()) >= ros_config_->max_pixel_points_){  
                    break; //源码是有；break
                } 
                    
                depth_map_list_.back()->depth_map_[point->position_].push_back(point);

                if(point->vec_(2) > depth_map_list_.back()->max_depth_all_[point->position_]){

                    depth_map_list_.back()->max_depth_all_[point->position_] = point->vec_(2);
                    depth_map_list_.back()->max_depth_index_all_[point->position_]
                            = depth_map_list_.back()->depth_map_[point->position_].size() - 1;
                }

                if(point->vec_(2) < depth_map_list_.back()->min_depth_all_[point->position_]
                        || depth_map_list_.back()->min_depth_all_[point->position_] < 10E-5){

                    depth_map_list_.back()->min_depth_all_[point->position_] = point->vec_(2);
                    depth_map_list_.back()->min_depth_index_all_[point->position_] = 
                            depth_map_list_.back()->depth_map_[point->position_].size() - 1;
                }

                break;

            default: 
                break;
        }
        // std::cout << "3-";
        buffer_.pop();
        // std::cout << " 4-\n";
    }
    // std::cout << "5-";
}

void DynObjFilter::filter(pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud, const Eigen::Matrix3d& rot, const Eigen::Vector3d& trans, double buffer_time){
    if(cloud == nullptr) return;

    auto time_begin = std::chrono::steady_clock::now();

    std::cout << "filter start\n";

    size_t cloud_size = cloud->size();
    dyn_tag_origin_.clear(),  dyn_tag_origin_.reserve(cloud_size),  dyn_tag_origin_.resize(cloud_size);
    dyn_tag_cluster_.clear(), dyn_tag_cluster_.reserve(cloud_size), dyn_tag_cluster_.resize(cloud_size);
    laser_cloud_dyn_obj_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
    laser_cloud_dyn_obj_->reserve(cloud_size);
    laser_cloud_dyn_obj_world_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
    laser_cloud_dyn_obj_world_->reserve(cloud_size);
    laser_cloud_stead_obj_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
    laser_cloud_stead_obj_->reserve(cloud_size);
    laser_cloud_dyn_obj_clus_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
    laser_cloud_dyn_obj_clus_->reserve(cloud_size);
    laser_cloud_stead_dyn_obj_clus_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
    laser_cloud_stead_dyn_obj_clus_->reserve(cloud_size);

    // pcl::PointCloud<pcl::PointXYZINormal> raw_points_world; //用于动态追踪, 存储body原始点
    // raw_points_world.reserve(cloud_size);
    // raw_points_world.resize(cloud_size);

    std::vector<int> index_vec(cloud_size);
    for(size_t i{0}; i < cloud_size; i++) index_vec[i] = static_cast<int>(i);

    std::vector<PointSoph*> points; //临时，存储判断类型后的像素点集合
    points.reserve(cloud_size);
    points.resize(cloud_size);

    PointSoph* p = point_soph_pointers_[index_point_soph_pointers_];

    //for test
    int num_0{-2}, num_1{-2}, num_2{-2}, num_3{-2}, num_4{-2}, num_5{-2}, num_6{-2};

    //处理原始像素， 分辨动态点， 结果储存于 std::vector<PointSoph*> points, 标志位dyn_flag_记录像素的状态
    std::for_each(std::execution::par, index_vec.begin(), index_vec.end(), [&](const int& i){
        // std::cout << "for_each 1\n";
        p[i].reset();
        Eigen::Vector3d p_body( cloud->points[i].x, cloud->points[i].y, cloud->points[i].z );
        Eigen::Vector3d p_world = rot * p_body + trans;
        int intensity = cloud->points[i].curvature;
        p[i].glob_ = p_world;
        p[i].dyn_flag_ = DynObjFlag::STATIC;
        p[i].rot_ = rot.transpose();
        p[i].transl_ = trans;
        p[i].time_= buffer_time;
        p[i].local_ = p_body;
        p[i].intensity_ = cloud->points[i].intensity;

        if(ros_config_->dataset_ == 0 && std::fabs(intensity-666) < 10E-4)
            p[i].is_distort_ = true;

        // p[i].dyn_flag_ = DynObjFlag::CASE1;
        
        if(false){}
        else if(invalidPointCheck(p_body)){
            // std::cout << "for_each 3\n";
            num_1++;
            p[i].dyn_flag_ = DynObjFlag::INVALID;
            dyn_tag_origin_[i] = 0;
            dyn_tag_cluster_[i] = -1;
        }
        else if(selfPointCheck(p_body)){
            // std::cout << "for_each 4\n";
            num_2++;
            p[i].dyn_flag_ = DynObjFlag::INVALID;
            dyn_tag_origin_[i] = 0;
        }
        else if(isCase_1(p[i])){
            // std::cout << "for_each 5\n";
            num_3++;
            p[i].dyn_flag_ = DynObjFlag::CASE1;
            dyn_tag_origin_[i] = 1;
        }
        else if(isCase_2(p[i])){
            num_4++;
            p[i].dyn_flag_ = DynObjFlag::CASE2;
            dyn_tag_origin_[i] = 1;
        }
        else if(isCase_3(p[i])){
            num_5++; 
            p[i].dyn_flag_ = DynObjFlag::CASE3;
            dyn_tag_origin_[i] = 1;
        }
        else{
            // std::cout << "for_each 6\n";
            num_6++;
            dyn_tag_origin_[i] = 0;
        }

        points[i] = &p[i];

        // std::cout << "for_each 8\n";
    });

    std::cout << "filter end\n";

    //遍历points，输出结果用于可视化和聚类
    for(size_t i{0}; i < cloud_size; i++){
        
        pcl::PointXYZINormal po_local, po_global;

        po_local.x = points[i]->local_[0];
        po_local.y = points[i]->local_[1];
        po_local.z = points[i]->local_[2];
        po_local.intensity = points[i]->intensity_;

        po_global.x = points[i]->glob_[0];
        po_global.y = points[i]->glob_[1];
        po_global.z = points[i]->glob_[2];

        // raw_points_world[i] = po_local;

        switch(points[i]->dyn_flag_){
            
            case DynObjFlag::CASE1:
                po_local.normal_x = 1;
                laser_cloud_dyn_obj_->push_back(po_local);
                laser_cloud_dyn_obj_world_->push_back(po_global);
                break;
            
            case DynObjFlag::CASE2:
                po_local.normal_y = points[i]->occu_times_;
                laser_cloud_dyn_obj_->push_back(po_local);
                laser_cloud_dyn_obj_world_->push_back(po_global);
                break;

            case DynObjFlag::CASE3:
                po_local.normal_z = points[i]->is_occu_times_;
                laser_cloud_dyn_obj_->push_back(po_local);
                laser_cloud_dyn_obj_world_->push_back(po_global);
                break;

            default:
                laser_cloud_stead_obj_->push_back(po_global);
                break;
        }
    }

    // if(ros_config_->cluster_coupled_ || ros_config_->cluster_future_){

    //     dyn_obj_cluster_.clusterProcess(dyn_tag_cluster_, *laser_cloud_dyn_obj_, raw_points_world, rot, trans);


    // }

    std::cout << "filter end 1\n";

    bufferPoints(points, index_vec);
    std::cout << "filter end 2\n";
    bufferDepthMap(buffer_time);

    if(laser_cloud_dyn_obj_->size() > 0){

        std::cout << "size :" << laser_cloud_dyn_obj_->size() << " "
            << laser_cloud_dyn_obj_world_->size() << " " << laser_cloud_stead_obj_->size()  << " | "
            << num_0  << " " << num_1 << " " << num_2 << " " << num_3 << " " << num_4 << " " << num_5 << " " << num_6 << " | "
            << index_vec.size() << " | "
            << num_10 << " " << num_11 << " " << num_12 << " " << num_13 << " " << num_14 << "\n";
    }
    
    num_10 = num_11 = num_12 = num_13 = num_14 = -1;

    auto time_consumed = std::chrono::steady_clock::now() - time_begin;
    std::cout << "filter time consumed: " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(time_consumed).count() << "ms\n";

    std::cout << "buffer_: " << buffer_.size() << "  depth_map_list_:" << depth_map_list_.size() << "\n----------------\n";


    index_point_soph_pointers_ = (index_point_soph_pointers_ + 1) % max_pointers_num_;
}

bool DynObjFilter::isCase_0(PointSoph& p){
    return true;
}
}

