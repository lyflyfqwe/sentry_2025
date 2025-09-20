#ifndef DYN_OBJ_FILTER_H
#define DYN_OBJ_FILTER_H

#include <chrono>
#include <deque>

#include <pcl_conversions/pcl_conversions.h>

#include <sensor_msgs/msg/point_cloud2.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include "livox_ros_driver2/msg/custom_msg.hpp" //for test

#include "m_detector/dyn_obj_cluster.h"
#include "m_detector/depth_map.h"
#include "m_detector/config.h"
#include "m_detector/parallel_q.h"

namespace m_detector{
class DynObjFilter: public rclcpp::Node{
private:
    Config::Ptr ros_config_;
    int max_ind_, hor_num_{MAX_1D}, ver_num_{MAX_1D_HALF};
    std::deque<pcl::PointCloud<pcl::PointXYZINormal>::Ptr> pcl_his_list_;

    pcl::PointCloud<pcl::PointXYZINormal>::Ptr laser_cloud_dyn_obj_; //动态点(原始)
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr laser_cloud_dyn_obj_world_;  //动态点(世界坐标)
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr laser_cloud_stead_obj_;  //筛选动态点后剩下的点
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr laser_cloud_dyn_obj_clus_;
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr laser_cloud_stead_dyn_obj_clus_;

    std::vector<int> pos_offset_;

    int map_cons_hor_num1_, map_cons_ver_num1_;
    int map_cons_hor_num2_, map_cons_ver_num2_;
    int map_cons_hor_num3_, map_cons_ver_num3_;
    int interp_hor_num_, interp_ver_num_;
    int occ_hor_num2_, occ_ver_num2_;
    int occ_hor_num3_, occ_ver_num3_;
    int depth_cons_hor_num2_, depth_cons_ver_num2_;
    int depth_cons_hor_num3_, depth_cons_ver_num3_;

    size_t index_point_soph_pointers_{0}; //point_soph_pointers的索引

    int case1_num_{0}, case2_num_{0}, case3_num_{0};  //对应三种不同的移动状态

    Parallel_Q<PointSoph*> buffer_;
    std::vector<PointSoph*> point_soph_pointers_;  //init()时即分配固定最大size: max_pointers_num, 每个元素即一帧所有点云
    DynObjCluster dyn_obj_cluster_;
    std::vector<int> dyn_tag_origin_, dyn_tag_cluster_;
    std::deque<DepthMap::Ptr> depth_map_list_; //buffer转换后的载体

    int pixel_fov_up_, pixel_fov_down_, pixel_fov_cut_, pixel_fov_left_, pixel_fov_right_;
    int max_pointers_num_;
    float cutoff_value_{0.0};
    int enlarge_distort_{4}; //扩大扭曲

    int buffer_depth_map_index_{0};

private: //回调部分
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_cloud_body_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_odom_body_;
    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_cloud_body_, pub_cloud_world_;

    std::deque<Eigen::Matrix3d> buffer_rot_;
    std::deque<Eigen::Vector3d> buffer_pos_;
    std::deque<double> buffer_time_;
    std::deque<std::shared_ptr<pcl::PointCloud<pcl::PointXYZINormal>>> buffer_cloud_;

private: //test
    int num_10{-2}, num_11{-2}, num_12{-2}, num_13{-2}, num_14{-2};

    rclcpp::Subscription<livox_ros_driver2::msg::CustomMsg>::SharedPtr sub_pcl_livox_;

    sensor_msgs::msg::PointCloud2::SharedPtr msg_trans_;
    

public:
    DynObjFilter(std::string node_name, rclcpp::NodeOptions option);

protected:
    void convertCustomMsg2CloudMsg(const livox_ros_driver2::msg::CustomMsg::SharedPtr& msg_in, 
            sensor_msgs::msg::PointCloud2::SharedPtr& cloud_out);

    void convertCustomMsg2PointCloud(const livox_ros_driver2::msg::CustomMsg::SharedPtr& msg_in, 
            pcl::PointCloud<pcl::PointXYZINormal>::Ptr& cloud_out);

    //限制点的范围
    bool invalidPointCheck(const Eigen::Vector3d& point);
    //排除自身点
    bool selfPointCheck(const Eigen::Vector3d& point);

    //更新 vec_ hor_ind_ ver_ind_ position_, PointSoph初始化时，上述值均没有变化
    //参数 depth_index 对应不同帧的depthmap。
    //由于己方在运动, 视野在变化， 所以在不同帧的depthmap里， 同一个PointSoph的上述值都是不同的
    void sphericalProjection(PointSoph& p, int depth_index, const Eigen::Matrix3d& rot, const Eigen::Vector3d& trans, PointSoph& p_spherical);

    bool checkVerFOV(const PointSoph& p, const DepthMap& map);

    //检查depthmap: map里，PointSoph:p 四周最大，最小的深度并返回 min_depth max_depth
    void checkNeighbor(const PointSoph& p, const DepthMap& map, float& min_depth, float& max_depth);

    //遮挡判定
    bool enterCase_1(const PointSoph& p, const DepthMap& map);

    // 返回 -1 表示没有点，-2 表示没有三角区但有点
    float depthMapInterpolationStatic(PointSoph& p, int map_index, const std::vector<std::vector<PointSoph*>>& depth_map);

    //待验证
    bool checkMapConsistencyCase_1(PointSoph& p, const DepthMap& map, bool is_interp);

    bool falseRejestionCase_1(PointSoph& p, const DepthMap& map);

    bool isCase_1(PointSoph& p); //平移

    bool enterCase_2(const PointSoph& p, const DepthMap& map);

    // -1 denotes no points, -2 denotes no triangular > 1000 denotes gauss interpolation 
    float depthMapInterpolationAll(PointSoph& p, const std::vector<std::vector<PointSoph*>>& depth_map);

    bool checkMapConsistencyCase_2(PointSoph& p, const DepthMap& map, bool is_interp);

    bool isOccludedCase_2(const PointSoph& p, const PointSoph& p_occ);

    bool checkDepthConsistencyCase_2(const PointSoph& p, const DepthMap& map);

    bool searchPointOculudingPCase_2(PointSoph& p, const DepthMap& map);

    bool checkVelCase_2(float v1, float v2, double delta_t);

    bool isCase_2(PointSoph& p); //远离

    bool enterCase_3(const PointSoph& p, const DepthMap& map);

    bool checkMapConsistencyCase_3(PointSoph& p, const DepthMap& map, bool is_interp);

    bool isOccludedCase_3(const PointSoph& p, const PointSoph& p_occ);

    bool checkDepthConsistencyCase_3(const PointSoph& p, const DepthMap& map);

    bool searchPointOculudingPCase_3(PointSoph& p, const DepthMap& map);

    bool checkVelCase_3(float v1, float v2, double delta_t);

    bool isCase_3(PointSoph& p);  //靠近 

    //将像素集合装入双端队列
    void bufferPoints(std::vector<PointSoph*>& points, std::vector<int>& index_vec);

    void bufferDepthMap(double cur_time);

    // 参数cloud 未转frame的点云，原始frame:body
    void filter(pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud, const Eigen::Matrix3d& rot, const Eigen::Vector3d& trans, double buffer_time);

    //for test
    bool isCase_0(PointSoph& p);
};
}

#endif