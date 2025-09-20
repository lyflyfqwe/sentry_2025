#include "path_motion_planning/nav_core/nav_local_track.h"

namespace path_motion_planning{
namespace nav_core{

bool NavLocalTrack::selectPosFromGPath(std::vector<Eigen::Vector3d>& safe_p, bool& safe_state){
    path_from_g_.lock();
    auto path_g = path_from_g_.getData();
    path_from_g_.unlock();
    if(path_g.empty()){
        debug_tools::Debug().print(".... path_g empty");
        return false;
    }
    debug_tools::Debug().print("path_g size:", static_cast<int>(path_g.size()));

    path_3i_.clear();
    std::vector<double> path_v;
    safe_state = true; //判断是否需要进入跟随模式
    Eigen::Vector3i cur_safe_i;

    for(size_t i{0}; i < path_g.size(); i++){
        auto pos_4d = path_g.at(i);
        Eigen::Vector3d pos_3d = Eigen::Vector3d(pos_4d.x(), pos_4d.y(), pos_4d.z()) - lidar_p_;
        auto pos_3i = map_->getGridIndexIgnoreAvailable(pos_3d);
        if(!map_->isIndexVaild(pos_3i)) continue;

        if(map_->findSafeClosed(pos_3i, 6, cur_safe_i)){
            path_3i_.push_back(cur_safe_i);
            path_v.push_back(pos_4d.w());
        } 
    }
    debug_tools::Debug().print("path_3i size:", static_cast<int>(path_3i_.size()));

    if(path_3i_.empty()){
        debug_tools::Debug().print("findSafeClosed L failed path_3i_.empty() 111111111111111111111111111111");
        return false;
    }

    auto it_closed{path_3i_.begin()};
    double dis_closed{std::numeric_limits<double>::max()};
    int i{0}, i_closed{0};
    for(auto it{it_closed}; it != path_3i_.end(); it++){
        i++;
        auto diff = *it -*it_closed;
        auto dis_cur = diff.x()*diff.x() + diff.y()*diff.y() + diff.z()*diff.z(); 
        if(dis_cur < dis_closed){
            dis_closed = dis_cur;
            it_closed = it;
            i_closed = i;
        }
    }

    bool not_appear_del_cost{true}; //标志位，判断前方是否出现sdf大于del_cost的路径点

    for(size_t k{static_cast<size_t>(i_closed)}; k < path_v.size(); k++){ 
        auto value = path_v.at(k);
        if(value > l_cfg_.sec_cfg.dead_cost) not_appear_del_cost = false;
        if(path_v.at(k) > l_cfg_.sec_cfg.safe_state_cost) safe_state = false;
    }
    debug_tools::Debug().print("...", safe_state? "[safe_state]" : "[not safe_state]");

    safe_i_.main.is_set = true;
    safe_i_.main.start = robot_.ind;
    safe_p.push_back(map_->getGridCubeCenterIgnoreAvailable(safe_i_.main.start) + lidar_p_);

    safe_i_.end = path_3i_.back();
    safe_p.push_back(map_->getGridCubeCenterIgnoreAvailable(safe_i_.end) + lidar_p_);

    controller_->setSpeedRate(safe_state? cur_spd_rate_ : 0.5); //非安全，采用低速
    robot_.cas.is_safe = safe_state;

    return not_appear_del_cost;
}

bool NavLocalTrack::seachPath(std::vector<Eigen::Vector3d>& path){
    if(path_3i_.empty()) return false;

    std::vector<Eigen::Vector3i> path_3i_out;
    auto plan_result = safe_i_.main.is_set && searcher_t_->plan(safe_i_.main.start, safe_i_.end, path_3i_, path_3i_out);
    if(!plan_result){
        debug_tools::Debug().print(".... [Local] seachPath failed");
        return false;
    }
    
    std::vector<Eigen::Vector3d> path_res;
    for(auto p_i : path_3i_out) path_res.push_back(map_->getGridCubeCenterIgnoreAvailable(p_i) + lidar_p_);
    path = interpolate(path_res, nav_cfg_.resolution); //输出的原始只有关键点，故均匀插入点 
    return true;
}

void NavLocalTrack::init(){
    NavLocal::init();
    altitude_map::AltitudeMap::Ptr map_ptr = map_;
    searcher_t_ = std::make_shared<path_searcher::ThetaStarTrack>(map_ptr, l_cfg_.sec_cfg.v_cost, l_cfg_.sec_cfg.num_limit);
    searcher_t_->setErrorZThres(l_cfg_.sec_cfg.z_error_thres);
    searcher_t_->setDelThres(l_cfg_.sec_cfg.dead_cost);
    searcher_t_->setNeighBourRadius(l_cfg_.sec_cfg.neighbour_radius);

    debug_tools::Debug().print("NavLocalTrack::init");
}

void NavLocalTrack::updatePathLForControl(const std::vector<Eigen::Vector3d>& path){
    controller_->updatePathL(path);
}
} //namespace nav_core
} //namespace path_motion_planning