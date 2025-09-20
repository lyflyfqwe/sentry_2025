#ifndef CONFIG_H
#define CONFIG_H

#include <fmt/color.h>
#include <memory>
#include <rclcpp/rclcpp.hpp>

namespace m_detector{
class Config{
public:
    typedef std::shared_ptr<Config> Ptr;
    //load params begin
    double buffer_delay_, depth_map_dur_, frame_dur_;
    int buffer_size_, points_num_perframe_, max_depth_map_num_, max_pixel_points_, dataset_;

    float self_x_f_, self_x_b_, self_y_l_, self_y_r_;
    float blind_dis_;
    float fov_up_, fov_down_, fov_cut_, fov_left_, fov_right_;

    int check_neighbor_range_;
    bool stop_object_detect_;
    float depth_thr1_, enter_min_thr1_, enter_max_thr1_;
    float map_cons_depth_thr1_, map_cons_hor_thr1_, map_cons_ver_thr1_, map_cons_hor_dis1_, map_cons_ver_dis1_;

    float depth_cons_depth_thr1_, depth_cons_depth_max_thr1_;
    float depth_cons_hor_thr1_, depth_cons_ver_thr1_;

    float enlarge_z_thr1_, enlarge_angle_, enlarge_depth_;

    int occluded_map_thr1_;
    bool case1_interp_en_;
    float k_depth_min_thr1_, d_depth_min_thr1_, k_depth_max_thr1_, d_depth_max_thr1_;

    float v_min_thr2_, acc_thr2_;
    float map_cons_depth_thr2_, map_cons_hor_thr2_, map_cons_ver_thr2_;
    float occ_depth_thr2_, occ_hor_thr2_, occ_ver_thr2_;

    float depth_cons_depth_thr2_, depth_cons_depth_max_thr2_, depth_cons_hor_thr2_, depth_cons_ver_thr2_;
    float k_depth2_;
    int occluded_times_thr2_;
    bool case2_interp_en_;

    float k_depth_max_thr2_, d_depth_max_thr2_, v_min_thr3_, acc_thr3_;
    float map_cons_depth_thr3_, map_cons_hor_thr3_, map_cons_ver_thr3_;
    float occ_depth_thr3_, occ_hor_thr3_, occ_ver_thr3_;
    float depth_cons_depth_thr3_, depth_cons_depth_max_thr3_, depth_cons_hor_thr3_, depth_cons_ver_thr3_;

    float k_depth3_;
    int occluding_times_thr3_;
    bool case3_interp_en_;
    float k_depth_max_thr3_, d_depth_max_thr3_;

    float interp_hor_thr_, interp_ver_thr_, interp_thr1_, interp_static_max_, interp_start_depth1_;
    float interp_kp1_, interp_kd1_, interp_thr2_, interp_thr3_;

    bool dyn_filter_en_, debug_publish_;
    int laserCloudSteadObj_accu_limit_;
    float voxel_filter_size_;
    bool cluster_coupled_, cluster_future_;

    int cluster__extend_pixel_, cluster__min_pixel_number_;
    float cluster__thrustable_thresold_, cluster__Voxel_revolusion_;
    bool cluster__debug_en_;

    float ver_resolution_max_, hor_resolution_max_, buffer_dur_;
    int point_index_;
    std::string frame_id_;

    std::string topic_cloud_body_, topic_odom_body_;
    //load params end

    Config(rclcpp::Node* node, std::string name_space){
        node_ptr_ = node;

        loadParam<std::string>(name_space + "topic_cloud_body", topic_cloud_body_, "/cloud_registered_body");
        loadParam<std::string>(name_space + "topic_odom_body", topic_odom_body_, "/point_lio_odom");

        loadParam<double>(name_space + "buffer_delay", buffer_delay_, 0.1);
        loadParam<int>(name_space + "buffer_size", buffer_size_, 100000);
        loadParam<int>(name_space + "points_num_perframe", points_num_perframe_, 30000); //30000
        loadParam<double>(name_space + "depth_map_dur", depth_map_dur_, 0.4);
        loadParam<int>(name_space + "max_depth_map_num", max_depth_map_num_, 3);//深度图的最大数量  5
        loadParam<int>(name_space + "max_pixel_points", max_pixel_points_, 20); //20
        loadParam<double>(name_space + "frame_dur", frame_dur_, 0.02);
        loadParam<int>(name_space + "dataset", dataset_, 4);

        loadParam<float>(name_space + "self_x_f", self_x_f_, 0.0f);
        loadParam<float>(name_space + "self_x_b", self_x_b_, 0.0f);
        loadParam<float>(name_space + "self_x_l", self_y_l_, 0.0f);
        loadParam<float>(name_space + "self_y_r", self_y_r_, 0.0f);
        loadParam<float>(name_space + "blind_dis", blind_dis_, 0.1);
        loadParam<float>(name_space + "fov_up", fov_up_, 52.0);
        loadParam<float>(name_space + "fov_down", fov_down_, -7.0);
        loadParam<float>(name_space + "fov_cut", fov_cut_, -20.0);
        loadParam<float>(name_space + "fov_left", fov_left_, 180.0);
        loadParam<float>(name_space + "fov_right", fov_right_, -180.0);

        loadParam<int>(name_space + "check_neighbor_range", check_neighbor_range_, 1); //1
        loadParam<bool>(name_space + "stop_object_detect", stop_object_detect_, false);
        loadParam<float>(name_space + "depth_thr1", depth_thr1_, 0.5);
        loadParam<float>(name_space + "enter_min_thr1", enter_min_thr1_, 0.5);
        loadParam<float>(name_space + "enter_max_thr1", enter_max_thr1_, 0.5);
        loadParam<float>(name_space + "map_cons_depth_thr1", map_cons_depth_thr1_, 0.5f);
        loadParam<float>(name_space + "map_cons_hor_thr1", map_cons_hor_thr1_, 0.05);
        loadParam<float>(name_space + "map_cons_ver_thr1", map_cons_ver_thr1_, 0.05);
        loadParam<float>(name_space + "map_cons_hor_dis1", map_cons_hor_dis1_, 0.3);
        loadParam<float>(name_space + "map_cons_ver_dis1", map_cons_ver_dis1_, 0.6);

        loadParam<float>(name_space + "depth_cons_depth_thr1", depth_cons_depth_thr1_, 0.5);
        loadParam<float>(name_space + "depth_cons_depth_max_thr1", depth_cons_depth_max_thr1_, 1.0);
        loadParam<float>(name_space + "depth_cons_hor_thr1", depth_cons_hor_thr1_, 0.05);
        loadParam<float>(name_space + "depth_cons_ver_thr1", depth_cons_ver_thr1_, 0.05);

        loadParam<float>(name_space + "enlarge_z_thr1", enlarge_z_thr1_, -2.5);
        loadParam<float>(name_space + "enlarge_angle", enlarge_angle_, 2.0);
        loadParam<float>(name_space + "enlarge_depth", enlarge_depth_, 3.0);

        loadParam<int>(name_space + "occluded_map_thr1", occluded_map_thr1_, 1); //遮挡阈值 3
        loadParam<bool>(name_space + "case1_interp_en", case1_interp_en_, true);
        loadParam<float>(name_space + "k_depth_min_thr1", k_depth_min_thr1_, 0.0f);  //未指定
        loadParam<float>(name_space + "d_depth_min_thr1", d_depth_min_thr1_, 0.15f); //未指定
        loadParam<float>(name_space + "k_depth_max_thr1", k_depth_max_thr1_, 0.0f);  //未指定
        loadParam<float>(name_space + "d_depth_max_thr1", d_depth_max_thr1_, 0.15f); //未指定

        loadParam<float>(name_space + "v_min_thr2", v_min_thr2_, 0.5);
        loadParam<float>(name_space + "acc_thr2", acc_thr2_, 7.0);
        loadParam<float>(name_space + "map_cons_depth_thr2", map_cons_depth_thr2_, 0.3);
        loadParam<float>(name_space + "map_cons_hor_thr2", map_cons_hor_thr2_, 0.02);
        loadParam<float>(name_space + "map_cons_ver_thr2", map_cons_ver_thr2_, 0.06);
        loadParam<float>(name_space + "occ_depth_thr2", occ_depth_thr2_, 0.3);
        loadParam<float>(name_space + "occ_hor_thr2", occ_hor_thr2_, 0.02);
        loadParam<float>(name_space + "occ_ver_thr2", occ_ver_thr2_, 0.06);

        loadParam<float>(name_space + "depth_cons_depth_thr2", depth_cons_depth_thr2_, 0.1);
        loadParam<float>(name_space + "depth_cons_depth_max_thr2", depth_cons_depth_max_thr2_, 0.5);
        loadParam<float>(name_space + "depth_cons_hor_thr2", depth_cons_hor_thr2_, 0.03);
        loadParam<float>(name_space + "depth_cons_ver_thr2", depth_cons_ver_thr2_, 0.06);
        loadParam<float>(name_space + "k_depth2", k_depth2_, 0.005f);
        loadParam<int>(name_space + "occluded_times_thr2", occluded_times_thr2_, 3);
        loadParam<bool>(name_space + "case2_interp_en", case2_interp_en_, false);

        loadParam<float>(name_space + "k_depth_max_thr2", k_depth_max_thr2_, 0.0f);
        loadParam<float>(name_space + "d_depth_max_thr2", d_depth_max_thr2_, 0.15f);
        loadParam<float>(name_space + "v_min_thr3", v_min_thr3_, 0.5f);
        loadParam<float>(name_space + "acc_thr3", acc_thr3_, 1.0f);
        loadParam<float>(name_space + "map_cons_depth_thr3", map_cons_depth_thr3_, 0.15f);
        loadParam<float>(name_space + "map_cons_hor_thr3", map_cons_hor_thr3_, 0.02f);
        loadParam<float>(name_space + "map_cons_ver_thr3", map_cons_depth_thr3_, 0.01f);
        loadParam<float>(name_space + "occ_depth_thr3", occ_depth_thr3_, 0.15f);
        loadParam<float>(name_space + "occ_hor_thr3", occ_hor_thr3_, 0.02f);
        loadParam<float>(name_space + "occ_ver_thr3", occ_ver_thr3_, 0.01f);
        loadParam<float>(name_space + "depth_cons_depth_thr3", depth_cons_depth_thr3_, 0.5f);
        loadParam<float>(name_space + "depth_cons_depth_max_thr3", depth_cons_depth_max_thr3_, 0.5f);
        loadParam<float>(name_space + "depth_cons_hor_thr3", depth_cons_hor_thr3_, 0.02f);
        loadParam<float>(name_space + "depth_cons_ver_thr3", depth_cons_ver_thr3_, 0.01f);

        loadParam<float>(name_space + "k_depth3", k_depth3_, 0.005f);
        loadParam<int>(name_space + "occluding_times_thr3", occluding_times_thr3_, 3);
        loadParam<bool>(name_space + "case3_interp_en", case3_interp_en_, false);
        loadParam<float>(name_space + "k_depth_max_thr3", k_depth_max_thr3_, 0.0f);  //未指定
        loadParam<float>(name_space + "d_depth_max_thr3", d_depth_max_thr3_, 0.15f); //未指定

        loadParam<float>(name_space + "interp_hor_thr", interp_hor_thr_, 0.03);
        loadParam<float>(name_space + "interp_ver_thr", interp_ver_thr_, 0.03);
        loadParam<float>(name_space + "interp_thr1", interp_thr1_, 0.5);
        loadParam<float>(name_space + "interp_static_max", interp_static_max_, 10.0f);
        loadParam<float>(name_space + "interp_start_depth1", interp_start_depth1_, 10.0f);
        loadParam<float>(name_space + "interp_kp1", interp_kp1_, 0.1f);
        loadParam<float>(name_space + "interp_kd1", interp_kd1_, 0.5f);
        loadParam<float>(name_space + "interp_thr2", interp_thr2_, 0.25f);
        loadParam<float>(name_space + "interp_thr3", interp_thr3_, 0.05f);

        loadParam<bool>(name_space + "dyn_filter_en", dyn_filter_en_, true);
        loadParam<bool>(name_space + "debug_publish", debug_publish_, true);  //未指定
        loadParam<int>(name_space + "laserCloudSteadObj_accu_limit", laserCloudSteadObj_accu_limit_, 5);
        loadParam<float>(name_space + "voxel_filter_size", voxel_filter_size_, 0.1f);  //未指定
        loadParam<bool>(name_space + "cluster_coupled", cluster_coupled_, true);
        loadParam<bool>(name_space + "cluster_future", cluster_future_, true);

        loadParam<int>(name_space + "cluster_extend_pixel", cluster__extend_pixel_, 2);
        loadParam<int>(name_space + "cluster_min_pixel_number", cluster__min_pixel_number_, 4);
        loadParam<float>(name_space + "cluster_thrustable_thresold", cluster__thrustable_thresold_, 0.3f);
        loadParam<float>(name_space + "cluster_Voxel_revolusion", cluster__Voxel_revolusion_, 0.3f);
        loadParam<bool>(name_space + "cluster_debug_en", cluster__debug_en_, false);  //未指定
        
        loadParam<float>(name_space + "ver_resolution_max", ver_resolution_max_, 0.04); //0.04  0.2
        loadParam<float>(name_space + "hor_resolution_max", hor_resolution_max_, 0.025); //0.025  0.1
        loadParam<float>(name_space + "buffer_dur", buffer_dur_, 0.1f);  //未指定
        loadParam<int>(name_space + "point_index", point_index_, -1);
        loadParam<std::string>(name_space + "frame_id", frame_id_, "camera_init");  //未指定
    }

private:
    rclcpp::Node* node_ptr_;
    
    template<typename T> void loadParam(std::string param_name, T &param_value, T default_value){
        try{
            node_ptr_->declare_parameter<T>(param_name, default_value);
            node_ptr_->get_parameter<T>(param_name, param_value);
            fmt::print(fmt::v8::fg(fmt::color::blue_violet), "[Config] load param {} = {}\n", param_name, param_value);
        }catch (const rclcpp::ParameterTypeException & e) {
            fmt::print(fmt::v8::fg(fmt::color::red), "[Config] load param {} failed. using default param\n", param_name);
        }
    }

};
}

#endif