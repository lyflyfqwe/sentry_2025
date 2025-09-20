#ifndef NAV_CORE_LOCAL_NAV_H
#define NAV_CORE_LOCAL_NAV_H

#include "path_planning/nav_core/nav_core.h"
#include "path_planning/util/vel_estimator.h"
#include "altitude_map/local_map.h"
#include "path_planning/path_searcher/theta_star.h"

namespace path_planning{
namespace nav_core{
class LocalNav: public NavCore{
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

    void flushLocalTempPath(std::vector<Eigen::Vector3d>& path_in){
        path_from_g_.setData(path_in);
    }

    /**
     * @brief 在由start,end 连接成的线段中均匀插值，返回点的集合
     */
    std::vector<Eigen::Vector2d> 
    interpolatePoints(const Eigen::Vector2d& start, const Eigen::Vector2d& end, double resolution);

    /**
     * @brief 找到path中符合下述条件的点坐标：属于surfel 索引尽可能靠后，点坐标满足sdf限制
     * @param ori 搜索起点 起点必须属于surfel
     * @param begin, end 路径的索引
     * @param target 终点集合,最多返回两个点，一远一近
     * @return success
     */
    bool findFarPosInsideBox(const Eigen::Vector3d& ori, std::vector<Eigen::Vector2d>::iterator begin, 
        std::vector<Eigen::Vector2d>::iterator end, std::vector<Eigen::Vector3d>& target);

public:
    typedef std::shared_ptr<LocalNav> Ptr;

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
     * @brief 从参考线中选取安全的起点和终点
     */
    bool selectPosFromLine(std::vector<Eigen::Vector2d>& line, std::vector<Eigen::Vector3d>& safe_p);

    /**
     * @brief 从全局路径中选取局部的起点和终点 
     * @return 0:success
     * @return 1:failed to find_start
     * @return 2:failed to find_end
     * @return 4:unknown_error
     */
    int selectPosFromPath(std::vector<Eigen::Vector3d>& safe_p);

    bool seachPath(std::vector<Eigen::Vector3d>& path);

    bool seachPath(std::vector<Eigen::Vector3d>& path, std::vector<Eigen::Vector3d>& safe_p);

    pcl::PointCloud<pcl::PointXYZI>::Ptr getCloud(){
        return map_->getMapTSDF();
    }

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

    LocalNav();

    /**
     * @brief 仅在测试时启用，用于初始化参数和分配内存
     */
    void init();
};

} //namespace nav_core
} //namespace path_planning

#endif