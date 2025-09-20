#ifndef RM_DECISION_RM_DECISION_H
#define RM_DECISION_RM_DECISION_H

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "rm_decision/enemy_tracker.h"
#include "rm_decision/polygon_region.h"
#include "rm_decision/tools.h"

namespace rm_decision{
class Regions{
private:
    std::vector<PolygonRegion> regions_;

public:
    enum TYPE{GUARD, GUARDS, GUARDT, CURE, TOWER, PLACE, UPHILL};
    enum ZONE {FIRST, SECOND, THIRD};

    PolygonRegion& operator[](TYPE type_name){
        if(type_name >= regions_.size()) regions_.resize(type_name+1);
        return regions_[type_name];
    }

    PolygonRegion& operator[](ZONE zone_name){
        if(zone_name >= regions_.size()) regions_.resize(zone_name+1);
        return regions_[zone_name];
    }

    const PolygonRegion& operator[](TYPE type_name) const{
        if(type_name >= regions_.size()) throw std::out_of_range("[Regions] out of range");
        return regions_[type_name];
    }

    const PolygonRegion& operator[](ZONE zone_name) const{
        if(zone_name >= regions_.size()) throw std::out_of_range("[Regions] out of range");
        return regions_[zone_name];
    }



    size_t size(){ return regions_.size(); }
}; //class Regions

class TaskTick{
private:
    int count_{};
    int count_limit_{};
    bool is_running_{false};
    

public:
    bool state_changed_{false};
    bool w_state_changed_{false};

    void setRunLimit(int limit){
        count_limit_ = limit;
        count_ = 0;
        is_running_ = true;
    }

    void fresh(){ 
        if(is_running_) count_++; 
    }

    bool timeout(){
        if(count_ >= count_limit_ || state_changed_) {
            if(state_changed_){
                state_changed_ = false;
                w_state_changed_ = true;
            }
            is_running_ = false;
        }
        return !is_running_; 
    }
}; //class TaskTick

class Range{
private:
    double min, max;

public:
    Range(double min_range=0, double max_range=0){
        reset(min_range, max_range);
    }

    void reset(double min_range, double max_range){
        min = min_range;
        max = max_range;
    }

    bool inside(double dis){
        return dis > min && dis <= max;
    }
}; //class Range

class RMMDecision: public Tools{
private:
    EnemyTracker::Ptr tracker_;
    std::atomic<Regions::TYPE> robot_state_{Regions::GUARD};

    struct SentryInfo{
        struct Odom{
            Eigen::Vector3d pos = Eigen::Vector3d::Zero();
            double yaw;
        }odom; //里程计相关

        struct Refer{
            int hp{100};
            int bollet{400};
            int energy{90};

            int hp_tower{0}, hp_tower_e{0};
            int hp_base{0}, hp_base_e{10};
            int control{0};
            int target_x{0}, target_y{0};
            double target_end_x{0},target_end_y{0};
        }ref; //裁判系统相关

        struct Nav{
            Eigen::Vector3d target_pos; //导航目标点
            Eigen::Vector3d highest_target_pos;
            bool is_goal_arrive{false}; //是否到达目标
            int l_fail_num{0};
        }nav; //导航相关

        struct EnforceYaw{
            Eigen::Vector3d target_pose;
            
            Eigen::Vector3d pos_enemy_world; //enemy map坐标系下真实坐标点
            double value;
        }ef; //强制yaw相关

        struct Track{
            Eigen::Vector3d pos;
            bool has_track_goal{false};
            bool track_state{false};
        }track;

        struct Guard
        {
            int guard{0};
        }guard;


    }info_;

    std::mutex mux_try_nav_server_;
    std::thread thd_time_count_, thd_decision_;
    bool is_running_{true};
    double distance{0};
    bool is_send{false};
    Eigen::Vector3d sleep_target;


    struct TaskMtx{
        std::mutex mtx;
        std::mutex lock_server;
        std::condition_variable cv;
        bool is_task_empty{true};

        inline void unlock(){
            {
                std::lock_guard<std::mutex> lock(mtx);
                is_task_empty = true;
            }
            cv.notify_one();
        }
    }task_mtx_;

    struct TickGroup{
        TaskTick task_run, task_wait, enfored_yaw_wait, track_itv, track_failed_forbid;
    }tick_;

    inline void updateTimeCount(){
        while(is_running_){
            tick_.enfored_yaw_wait.fresh();
            tick_.task_run.fresh();
            tick_.task_wait.fresh();
            tick_.track_itv.fresh();
            tick_.track_failed_forbid.fresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    inline void decisionCore();
    
    inline bool isGuard(){
        return robot_state_ != Regions::CURE && robot_state_ != Regions::TOWER;
    }

    inline bool isControlled(){
        return info_.ref.control == cfg_.ref.next_control_state;
    }

    inline bool turnControlled(){
        return info_.ref.control == cfg_.ref.state_change;
    }

public:
    typedef std::shared_ptr<RMMDecision> Ptr;
    Regions regions_;

    struct DecisionConfig{
        struct Nav{
            Range attack_range{0.0, 5.0}; //攻击范围
            Range track_range{3.0, 10.0}; //追击范围
            double target_dis_thres{1.0}; //导航目标距离到达阈值
            double target_yaw_thres{0.2}; //强制yaw判定为到达的阈值

            int max_l_failed_num{500};

            Eigen::Vector3d attact_tower_pos{ 5.64,0.37,-0.79};
        }nav;

        struct TimeCounter{
            int max_ef_miss{10}; //强制yaw丢失目标计数器阈值
            int max_task_run{100};
            int max_control_run{200};
            int max_task_wait{20};
            int track_itv_time{50};
            int track_failed_forbid{50}; //追击不可到达后，禁用追击时间
        }time;

        struct Refer{
            int lowwer_hp{200};
            int lowwer_bullet{30};
            int healthy_hp{300};
            int lowwer_energy{50};
            int next_control_state{1};
            int state_change{1};
        }ref;
    }cfg_;

    RMMDecision(){
        tracker_ = std::make_shared<EnemyTracker>();
    }

    void updateSentryInfo(const std::vector<int>& info);

    void updateRobotState();

    void updateAim(const Eigen::Vector3d& aim, const Eigen::Vector3d& v_pose, double v);

    void updateOdom(const Eigen::Vector3d& pos, double yaw);

    void updateLFailNum(const int result);

    void run();

    void stop(){
        is_running_ = false;
        task_mtx_.cv.notify_one();
        if(thd_time_count_.joinable()) thd_time_count_.join();
        if(thd_decision_.joinable()) thd_decision_.join();
    }

    void getEFYawCommand(int& flag, Eigen::Vector3d& target_pose, bool& spin, bool& ahead);

    bool getTargetPos(Eigen::Vector3d& pos, bool& is_track);

    double changeSpeed();

    /**
     * @brief 反馈请求导航服务后的结果
     * @param result 导航结果
     */
    void callBackNavReqResult(bool result, bool is_track);

}; //class RMMDecision
}// namespace rm_decision

#endif