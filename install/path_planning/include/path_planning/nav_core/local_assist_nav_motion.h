#ifndef NAV_CORE_LOCAL_ASSIST_NAV_MOTION_H
#define NAV_CORE_LOCAL_ASSIST_NAV_MOTION_H

#include "path_planning/nav_core/nav_core.h"
#include "path_planning/util/vel_estimator.h"
#include "altitude_map/local_map.h"
#include "path_planning/path_searcher/theta_star_assist.h"
#include "path_planning/motion_planning/motion_core.h"

namespace path_planning{
namespace nav_core{
class LocalAssistNavMotion: public NavCore{
private:
    PathData path_from_g_; //临时存储的全局路径

protected:
    altitude_map::LocalMap::Ptr map_;
    util::VelocityEstimator::Ptr vel_estimator_;

    Eigen::Vector3d lidar_p_; //雷达中心在全局的里程计
    Eigen::Vector3d robot_center_p_{}; Eigen::Matrix3d robot_center_m_; //全局机器人中心里程计
    Eigen::Vector3i robot_center_i_; //机器人中心在局部地图的索引

    Eigen::Vector3d robot_center_vel_{}; //全局坐标下,机器人中心的速度
    PosI2 safe_i_;

    /**
     * @brief 缓存用来参考的全局路径
     * @param path_in 输入的全局路径
     */
    void flushLocalTempPath(std::vector<Eigen::Vector3d>& path_in){
        path_from_g_.setData(path_in);
    }

    /**
     * @brief 在由start,end 连接成的线段中均匀插值，返回点的集合
     */
    std::vector<Eigen::Vector2d> 
    interpolatePoints(const Eigen::Vector2d& start, const Eigen::Vector2d& end, double resolution);

    /**
     * @brief 从参考线段中选择导航终点
     * @param safe_start safe_start_p global·坐标系
     * @param begin, end 路径的索引
     * @param target 终点 global·坐标系
     * @return success
     */
    bool selectTargetFromReferedLine(const Eigen::Vector3d& safe_start, std::vector<Eigen::Vector2d>::iterator begin, 
        std::vector<Eigen::Vector2d>::iterator end, Eigen::Vector3d& target);

    /**
     * @brief 从全局路径中选择导航终点
     * @param safe_start safe_start_p global·坐标系
     * @param begin, end 路径的索引
     * @param target 终点 global·坐标系
     * @return success
     */
    bool selectTargetFromReferedLine(const Eigen::Vector3d& safe_start, std::vector<Eigen::Vector3d>::iterator begin, 
            std::vector<Eigen::Vector3d>::iterator end, Eigen::Vector3d& target);

public:
    typedef std::shared_ptr<LocalAssistNavMotion> Ptr;

    struct LocalNavConfig{
        Eigen::Vector3d offset_robot_p; Eigen::Matrix3d offset_robot_m;
    }local_nav_cfg_;

    void updateMap(std::shared_ptr<pcl::PointCloud<pcl::PointXYZI>> &cloud_in, 
        const Eigen::Vector3d &odom_p, const Eigen::Matrix3d& odom_q, double update_t);

    /**
     * @brief 获取局部搜索的参考线，即起点和终点的连线
     * @param p_end 终点
     */
    std::vector<Eigen::Vector2d> 
    getReferLine(const Eigen::Vector3d& p_end, std::vector<Eigen::Vector3d>& line_path);

    /**
     * @brief 从参考的路径中获取起点和终点
     * @param line 参考的路径 2d
     */
    bool selectPosFromLine(std::vector<Eigen::Vector2d>& line, std::vector<Eigen::Vector3d>& safe_p);

    /**
     * @brief 从参考的路径中获取起点和终点，这里的参考路径是全局路径
     * @param safe_p 用于可视化的起点和终点 
     */
    bool selectPosFromLine(std::vector<Eigen::Vector3d>& safe_p);

    bool seachPath(std::vector<Eigen::Vector3d>& path, std::vector<Eigen::Vector3d>& safe_p);

    pcl::PointCloud<pcl::PointXYZI>::Ptr getCloud(){
        return map_->getMapTSDF();
    }

    // pcl::PointCloud<pcl::PointXYZ>::Ptr getCloud(){
    //     return map_->getMapOccupied();
    // }

    std::tuple<Eigen::Vector3d, Eigen::Quaterniond> getOdom(){
        auto qua = Eigen::Quaterniond(robot_center_m_);
        return {robot_center_p_, qua};
    }

    Eigen::Vector3d getVel(){
        return robot_center_vel_;
    }

    void setDiedCost(double cost){
        searcher_->setDelThres(cost);
    }

    LocalAssistNavMotion();

    /**
     * @brief 仅在测试时启用，用于初始化参数和分配内存
     */
    void init();
};

} //namespace nav_core
} //namespace path_planning

#endif