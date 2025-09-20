#ifndef NAV_CORE_GL_NAV_H
#define NAV_CORE_GL_NAV_H

#include "path_planning/nav_core/local_nav.h"
#include "path_planning/nav_core/local_assist_nav.h"
#include "altitude_map/global_map.h"

namespace path_planning{
namespace nav_core{
class GLNav: public LocalAssistNav{
private:
    PathData path_data_; //result of global-path searching 
    altitude_map::GlobalMap::Ptr map_g_;
    path_searcher::PathSearcher::Ptr searcher_g_; 

    PosI2 safe_i_g_;

public:
    typedef std::shared_ptr<GLNav> Ptr;

    struct GlobalNavConfig{
        Eigen::Vector3d bound_min{}, bound_max{};
        double resolution{};
        int safe_radius{};
        double safe_cost{};
        std::string pcd_file;
    }global_nav_cfg_;

    int selectPosG(const Eigen::Vector3d& pos, std::vector<Eigen::Vector3d>& safe_p);

    bool searchPathG(std::vector<Eigen::Vector3d>& path);

    /**
     * @brief 将搜索得到的全局路径作为局部路径搜索时设置起终点的参考
     */
    void replaceLocalTempPath(){
        path_data_.lock();
        auto path = path_data_.getData();
        path_data_.unlock();
        flushLocalTempPath(path);
    }

    GLNav();

    void initParams();

    void init();

    pcl::PointCloud<pcl::PointXYZI>::Ptr getCloudGlobal(){
        return map_g_->getMapTSDF(true);
    }

    void setDiedCostG(double cost){
        searcher_g_->setDelThres(cost);
    }
};

} //namespace nav_core
} //namespace path_planning

#endif