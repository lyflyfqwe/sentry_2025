#include "rm_decision/rmm_decision.h"

namespace rm_decision{

inline void RMMDecision::decisionCore(){
    while(is_running_){
        debug_tools::Debug().print(info_.ref.bollet,info_.ref.energy,info_.ref.hp,info_.ref.hp_tower_e);
        // std::cout<<"robot_state:"<<robot_state_<<std::endl;
        // std::cout<<"sentry_control:"<<cfg_.ref.next_control_state<<std::endl;
        // std::cout<<"serial_control:"<<info_.ref.control<<std::endl;
        // std::cout<<"is_control:"<<isControlled()<<std::endl;
        bool need_set_task{false};

        if(tick_.track_itv.timeout()){ //更新是否可以追击
            info_.track.has_track_goal = 
                tracker_->selectTarget(info_.track.pos.x(), info_.track.pos.y(), info_.track.pos.z());
            if(info_.track.has_track_goal){
                tick_.track_itv.setRunLimit(cfg_.time.track_itv_time);
                need_set_task = true;
            }
        }

        if(!info_.nav.is_goal_arrive){ //未到达目标
            if((info_.nav.target_pos - info_.odom.pos).norm() < cfg_.nav.target_dis_thres){ //到达目标
                if(isControlled()){
                    cfg_.ref.next_control_state +=1;
                }
                info_.nav.is_goal_arrive = true;
                tick_.task_wait.setRunLimit(cfg_.time.max_task_wait);
            }
            else if(tick_.task_run.timeout() || info_.nav.l_fail_num > cfg_.nav.max_l_failed_num){
                if(isControlled() && !tick_.task_run.w_state_changed_){
                    cfg_.ref.next_control_state +=1;
                }
                tick_.task_run.w_state_changed_ = false;
                need_set_task = true;  //超时 || l_fail_num超过阈值
            }
                    
        }
        else if(info_.nav.is_goal_arrive && tick_.task_wait.timeout()) need_set_task = true;

        if(need_set_task) task_mtx_.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void RMMDecision::updateSentryInfo(const std::vector<int>& info){
    if(info.size() != 7){
        return;
    }
    info_.ref.hp = info.at(0);
    info_.ref.bollet = info.at(1);
    info_.ref.hp_tower_e = info.at(2);
    info_.ref.control = info.at(3);
    info_.ref.target_x = info.at(4);
    info_.ref.target_y = info.at(5);
    info_.ref.energy = info.at(6);
    info_.ref.target_end_x = (double)info_.ref.target_x / 100.0;
    info_.ref.target_end_y = (double)info_.ref.target_y / 100.0;
    updateRobotState();
}

void RMMDecision::updateRobotState(){
    if(isControlled()){
        info_.nav.highest_target_pos = Eigen::Vector3d(info_.ref.target_end_x,info_.ref.target_end_y,0.0);
        if(turnControlled()){
            tick_.task_run.state_changed_ = true;
            cfg_.ref.state_change +=1;
        }
        if(regions_[Regions::FIRST].inside(info_.nav.highest_target_pos,1.0)){
            info_.nav.highest_target_pos = Eigen::Vector3d(info_.ref.target_end_x,info_.ref.target_end_y,10.0);
            robot_state_ = Regions::TOWER;
        }
        else if(info_.ref.target_end_x == 100.0){
            info_.nav.highest_target_pos = Eigen::Vector3d(info_.ref.target_end_x,info_.ref.target_end_y,0.0);
        }
        else{
            info_.nav.highest_target_pos = Eigen::Vector3d(info_.ref.target_end_x,info_.ref.target_end_y,1.0);
        }
        return;
    }
    else{
        if(info_.ref.hp < cfg_.ref.lowwer_hp || info_.ref.bollet < cfg_.ref.lowwer_bullet){
            if(robot_state_ != Regions::CURE){
                robot_state_ = Regions::CURE;
                tick_.task_run.state_changed_ = true;
            }
        }
        else if(info_.ref.hp > cfg_.ref.healthy_hp && info_.ref.bollet > cfg_.ref.lowwer_bullet && robot_state_ == Regions::CURE){
            if(info_.ref.hp_tower_e != 0){
                // robot_state_ = Regions::TOWER;
                robot_state_ = Regions::GUARD;
            }
            else{
                robot_state_ = Regions::UPHILL;
                // robot_state_ = Regions::GUARD;
            }
            tick_.task_run.state_changed_ = true;
        }
        if(robot_state_ == Regions::TOWER && info_.ref.hp_tower_e == 0){
            robot_state_ = Regions::GUARD;
            tick_.task_run.state_changed_ = true;
        }
        if(robot_state_ == Regions::UPHILL && regions_[Regions::UPHILL].inside(info_.odom.pos,0.1)){
            robot_state_ = Regions::GUARD;
            tick_.task_run.state_changed_ = true;
        }
        if(isGuard()){
            info_.guard.guard = (info_.guard.guard + 1) % 3;
            robot_state_ = static_cast<Regions::TYPE>(info_.guard.guard);
        }
    }
}

void RMMDecision::updateAim(const Eigen::Vector3d& aim, const Eigen::Vector3d& v_pose, double v){
    if(v <= 0){
        tracker_->updatePosition(aim.x(), aim.y(), aim.z(), v, false);
        info_.track.track_state = false;
        return;
    }
    auto dis = (aim - info_.odom.pos).norm();

    if(cfg_.nav.track_range.inside(dis)) tracker_->updatePosition(aim.x(), aim.y(), aim.z(), v, true);

    if(cfg_.nav.attack_range.inside(dis)){
        if(std::abs(Tools::getYaw(v_pose) - info_.odom.yaw) < cfg_.nav.target_yaw_thres) //yaw差角小于阈值
                tick_.enfored_yaw_wait.setRunLimit(cfg_.time.max_ef_miss);
        info_.ef.pos_enemy_world = aim;
        info_.ef.target_pose = v_pose;
    }
}

void RMMDecision::updateOdom(const Eigen::Vector3d& pos, double yaw){
    info_.odom.pos = pos;
    info_.odom.yaw = yaw;
}

void RMMDecision::updateLFailNum(const int result){
    if(result != 0) info_.nav.l_fail_num++;
}

void RMMDecision::run(){
    thd_time_count_ = std::thread(&RMMDecision::updateTimeCount, this);
    thd_decision_ = std::thread(&RMMDecision::decisionCore, this);
}

void RMMDecision::getEFYawCommand(int& flag, Eigen::Vector3d& target_pose, bool& spin, bool& ahead){
    if(isGuard()){
        if(!tick_.enfored_yaw_wait.timeout()){
                auto distance = (info_.odom.pos - info_.ef.pos_enemy_world).norm();
                flag = 0;
                if(cfg_.nav.attack_range.inside(distance) && cfg_.nav.track_range.inside(distance))flag = 1;
                else if(cfg_.nav.attack_range.inside(distance) && !cfg_.nav.track_range.inside(distance))flag = 2;
                spin = cfg_.nav.attack_range.inside(distance) && !cfg_.nav.track_range.inside(distance);
                target_pose = info_.ef.target_pose;
                ahead = false;
        }
        else {
            flag = spin = false;
        }
    }

    if(robot_state_ == Regions::TOWER){
        if((cfg_.nav.attact_tower_pos - info_.odom.pos).norm() < 0.3){
            flag = 2;
            spin = ahead = true;
        }
        else{
            flag = 0;
            spin = ahead = false;
        }
    }

    if(robot_state_ == Regions::CURE){
        flag = spin = ahead = false;
    }
    
}

bool RMMDecision::getTargetPos(Eigen::Vector3d& pos, bool& is_track){
    std::unique_lock<std::mutex> lock(task_mtx_.mtx);
    debug_tools::Debug().print("(getTargetPos) cv_wait_begin");
    task_mtx_.cv.wait(lock, [this]{std::cout<<"END"<<std::endl; return task_mtx_.is_task_empty || !is_running_; });
    debug_tools::Debug().print("(getTargetPos) cv_wait_end");
    
    if(!is_running_) return false;

    std::lock_guard<std::mutex> lock_server(task_mtx_.lock_server);

    if(isControlled()){
        info_.nav.target_pos = pos = info_.nav.highest_target_pos;
        task_mtx_.is_task_empty = false;
        tick_.task_run.setRunLimit(cfg_.time.max_control_run);
        is_track = false;
        return true;
    }
    ////巡逻状态 && 过去存在追击目标 && 从追击获取的点未过时
    else if(isGuard() && info_.track.has_track_goal && tick_.track_failed_forbid.timeout()){
            info_.nav.target_pos = pos = info_.track.pos;
            task_mtx_.is_task_empty = false;
            tick_.task_run.setRunLimit(cfg_.time.max_task_run);
            is_track = true;
            return true;
    }
    else if(regions_[robot_state_].randomPoint(pos)){
        info_.nav.target_pos = pos;
        task_mtx_.is_task_empty = false;
        tick_.task_run.setRunLimit(cfg_.time.max_task_run);
        is_track = false;
        return true;
    }
    return false;
}

double RMMDecision::changeSpeed(){
    if(info_.ref.energy >= cfg_.ref.lowwer_energy){
        if(robot_state_ == Regions::CURE || robot_state_ == Regions::TOWER){
            return 2.0;
        }
        else if(info_.track.has_track_goal){
            return 2.0;
        }
        else{
            return 2.0;
        }
    }
    else{
        return 2.0;
    }
}

void RMMDecision::callBackNavReqResult(bool result, bool is_track){
    std::lock_guard<std::mutex> lock_server(mux_try_nav_server_);
    debug_tools::Debug().print("callBackNavReqResult:", result);
    info_.nav.is_goal_arrive = false;
    if(result) info_.nav.l_fail_num = 0;
    else{
        if(is_track) tick_.track_failed_forbid.setRunLimit(cfg_.time.track_failed_forbid);
        task_mtx_.unlock();
    }
}
} //namespace rm_decision