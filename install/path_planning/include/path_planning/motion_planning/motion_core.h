#ifndef MOTION_PLANNING_MOTION_CORE_H
#define MOTION_PLANNING_MOTION_CORE_H

#include <condition_variable>
#include <vector>
#include <Eigen/Eigen>
#include <thread>
#include "debug_tools/debug_tools.h"
#include "path_planning/motion_planning/speed_tracker.h"

namespace path_planning{
namespace motion_planning{
class MutexData{
private:
    std::vector<Eigen::Vector3d> data;
    std::condition_variable cv;
    mutable std::mutex mutex;
    bool is_working{false};

public:
    void setData(const std::vector<Eigen::Vector3d>& path){
        std::unique_lock<std::mutex> lock(mutex);  
        cv.wait(lock, [this]{ return !is_working; });
        data = path;
    }

    std::vector<Eigen::Vector3d> getData(){
        std::lock_guard<std::mutex> lock(mutex);
        return data;
    }

    void lock(bool state){
        {
            std::lock_guard<std::mutex> lock(mutex);
            is_working = state;
        }
        cv.notify_all();
    }
};

class MotionCore{
private:
    MutexData path_;
    util::SpeedTracker::Ptr tracker_;
    Eigen::Vector3d robot_p_;  Eigen::Matrix3d robot_m_; //全局机器人中心里程计
    Eigen::Vector2d robot_speed_;

    struct GroupGlobal{
        Eigen::Vector3d pose_target, pos_target;
        bool is_new_data{false};
    }global_;

    std::vector<Eigen::Vector3d>::iterator 
    findClosedPosOfPath(std::vector<Eigen::Vector3d>::iterator begin, 
        std::vector<Eigen::Vector3d>::iterator end, const Eigen::Vector3d& robot_pos);

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef std::shared_ptr<MotionCore> Ptr;

    struct MotionConfig{
        int control_interval_ms; //控制时间间隔
        double max_speed, max_accel;
        int traj_ahead_index;
    }m_cfg_;

    void updateOdom(const Eigen::Vector3d& pos, const Eigen::Matrix3d& mat, const Eigen::Vector2d& spd);

    /**
     * @brief 更新局部路径
     */
    void updatePathL(const std::vector<Eigen::Vector3d>& path);

    /**
     * @brief 更新全局路径，更新后应将yaw旋转至全局路径的大致朝向
     */
    void updatePathG(const std::vector<Eigen::Vector3d>& path);

    Eigen::Vector3d getCommand();

    Eigen::Vector3d getCommandForEstiOffSet();

    void init();
};
} //namespace motion_planning
} //namespace path_planning

#endif