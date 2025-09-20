#ifndef NAV_CORE_GLOBAL_NAV_H
#define NAV_CORE_GLOBAL_NAV_H

#include <stack>
#include "path_planning/nav_core/nav_core.h"
#include "altitude_map/global_map.h"
#include "path_planning/path_searcher/theta_star.h"

namespace path_planning{
namespace nav_core{
class GlobalNav: public NavCore{
private:
    altitude_map::GlobalMap::Ptr map_;
    std::stack<Eigen::Vector3i> pos_safe_;

public:
    typedef std::shared_ptr<GlobalNav> Ptr;

    size_t selectPos(const Eigen::Vector3d& pos, Eigen::Vector3d& p_safe);

    bool seachPath(const PosI2& pos_safe, std::vector<Eigen::Vector3d>& path);

    bool seachPath(std::vector<Eigen::Vector3d>& path);

    pcl::PointCloud<pcl::PointXYZI>::Ptr getMap(){
        return map_->getMapTSDF(true);
    }

    GlobalNav();
    
    /**
     * @brief 仅在测试时启用，用于初始化参数和分配内存
     */
    void init(std::string pcd_file);
};
} //namespace nav_core
} //namespace path_planning

#endif