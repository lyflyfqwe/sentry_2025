#ifndef PATH_MOTION_PLANNING_NAV_CORE_NAV_GL_H
#define PATH_MOTION_PLANNING_NAV_CORE_NAV_GL_H

#include "path_motion_planning/nav_core/nav_local_track.h"
#include "altitude_map/global_map.h"

namespace path_motion_planning{
namespace nav_core{
/**
 * @brief global_planning with local_planning 
 */
class NavGL: public NavLocalTrack{
private:
    altitude_map::GlobalMap::Ptr map_g_;
    path_searcher::PathSearcher::Ptr searcher_g_;
    PosI2 safe_g_i_;

    /**
     * @brief 获取全局地图中机器人中心坐标对应的sdf
     * @return robot 位于地图外，返回0
     */
    inline double getRobotCenterSDF(){
        if(!map_g_->isInMap(robot_.pos)) return 0;
        auto index = map_g_->getGridIndex(robot_.pos);
        return map_g_->getSDFValue(index);
    }

public:
    typedef std::shared_ptr<NavGL> Ptr;

    struct LocalNavConfig{

        struct MapConfig{
            Eigen::Vector3d bound_min;
            Eigen::Vector3d bound_max;
            double resolution;
            double safe_cost;
            int safe_radius;
            std::string pcd_file;
        }m_cfg; //地图相关参数

        struct AltitudeMapConfig{
            double trun_radius{0.4}; // tsdf截断距离（类似于膨胀半径）默认0.4m
            int down_spread_dis{2}; // 障碍物向下传播的距离 默认2格
        }alti_cfg;

        struct SearchConfig{
            double v_cost{30};
            double dead_cost{0.5};
            int num_limit{10000};
        }sec_cfg; //路径搜索相关参数

    }nav_cfg_g_; //全局planning参数 

    /**
     * @brief 设置默认参数，仅用于测试
     */
    void setDefaultParams();

    void init();

    bool selectPosG(const Eigen::Vector3d& pos, std::vector<Eigen::Vector3d>& safe_p);

    /**
     * @brief 全局路径搜索
     * @param path 引用返回 path[x,y,z,cost] 
     * @param is_update_init_yaw 是否更新初始yaw朝向，移动前优先把yaw旋转到目标yaw
     */
    bool searchPathG(std::vector<Eigen::Vector4d>& path, bool is_update_init_yaw);

    pcl::PointCloud<pcl::PointXYZI>::Ptr getCloudG(){
        return map_g_->getMapTSDF(true);
    }

    /**
     * @brief 获取控制命令 重载 NavLocal std::tuple<Eigen::Vector3d, int, bool> getCommand_3d()
     * @return Eigen::Vector3d(x, y, r)
     * @return int robot_mode
     * @return bool 是否开启前哨站模式
     */
    std::tuple<Eigen::Vector3d, int, bool> getCommand_3d() override{
        auto safe_state = ef_yaw_.flag? (getRobotCenterSDF() < l_cfg_.sec_cfg.safe_state_cost) : robot_.cas.is_safe;
        auto chassis_mode = safe_state? (robot_.cas.is_spin? 2 : 0) : 1;
        return std::make_tuple(
            controller_->getCommand_3d(ef_yaw_.flag, ef_yaw_.target_pose), 
            chassis_mode,
            ef_yaw_.ahead);
    }

    /**
     * @brief 获取控制命令 重载 NavLocal std::tuple<Eigen::Vector<double, 5>, int, bool> getCommand_5d()
     * @return Eigen::Vector3d(x, y, r, mx, my)  mx, my 为map坐标系下的速度分量
     * @return bool safe_state
     * @return bool 是否开启前哨站模式
     */
    std::tuple<Eigen::Vector<double, 5>, int, bool> getCommand_5d() override{
        auto safe_state = ef_yaw_.flag? (getRobotCenterSDF() < l_cfg_.sec_cfg.safe_state_cost) : robot_.cas.is_safe;
        auto chassis_mode = safe_state? (robot_.cas.is_spin? 2 : 0) : 1;
        return std::make_tuple(
            controller_->getCommand_5d(ef_yaw_.flag, ef_yaw_.target_pose),
            chassis_mode,
            ef_yaw_.ahead);  
    }

}; //class NavGL
} //namespace nav_core
} //namespace path_motion_planning

#endif