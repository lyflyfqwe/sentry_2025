#ifndef MOTION_PLANNING_UTIL_SPEED_TRACKER_H
#define MOTION_PLANNING_UTIL_SPEED_TRACKER_H

#include <memory>
#include <Eigen/Eigen>
#include "debug_tools/debug_tools.h"

namespace path_planning{
namespace motion_planning{
namespace util{
class SpeedTracker{
private:
    int traj_ahead_index_;
    double max_speed_; //x,y分方向的最大速度
    double max_accel_; //x,y分方向的最大加速度 除于控制频率
    double control_interval_; //控制时间间隔 秒 

    inline double getYaw(const Eigen::Matrix3d& mat){
        return std::atan2(mat(1, 0), mat(0, 0));
    }

    inline double getYaw(const Eigen::Vector3d& v){
        return std::atan2(v.y(), v.x());
    }

    inline void normYaw(double& yaw){
        if(yaw > M_PI) yaw -= 2*M_PI;
        else if(yaw < -M_PI) yaw += 2*M_PI;
    }

public:
    typedef std::shared_ptr<SpeedTracker> Ptr;

    /**
     * @brief 更新速度和位置, 输出控制 x, y, r分量
     * @param path 输入的路径，第一个元素即为最靠近 robot_pos 的点
     * @param robot_pos 机器人中心坐标
     * @param robot_mat 机器人的姿态
     * @param robot_speed 机器人中心的速度 
     * @return Eigen::Vector3d(x, y, r)
     */
    Eigen::Vector3d updateState(const std::vector<Eigen::Vector3d>& path, const Eigen::Vector3d& pos_taregt, 
        Eigen::Vector3d& robot_pos, const Eigen::Matrix3d& robot_mat, Eigen::Vector2d& robot_speed){
        double yaw = getYaw(robot_mat);
        Eigen::Vector2d accel; //速度的变化量，即加速度
        Eigen::Vector3d speed; //输出的速度
        if((robot_pos-pos_taregt).norm() > 1 && traj_ahead_index_ < static_cast<int>(path.size())){
            Eigen::Vector3d pose_target = (path.at(traj_ahead_index_) - robot_pos).normalized();
            Eigen::Vector2d speed_target =  //目标速度即为最大速度
                Eigen::Vector2d(pose_target.x(), pose_target.y()).normalized() * max_speed_;
            Eigen::Vector2d speed_error = speed_target - robot_speed;

            for(int i{0}; i < 2; i++){
                if(std::abs(speed_error[i]) < max_accel_) accel[i] = speed_error[i]; //速度差异
                else accel[i] = speed_error[i]>0? max_accel_ : -max_accel_;
            }
            // debug_tools::Debug().print("a:", accel.x(), accel.y());

            auto yaw_diff = getYaw(pose_target) - yaw;
            normYaw(yaw_diff);

            speed.z() = std::abs(yaw_diff) < 0.15? 0 : -yaw_diff;
        }
        else{ //靠近终点减速
            double max_accel_slow = max_accel_ * 4;
            for(int i{0}; i < 2; i++){
                if(std::abs(robot_speed[i]) > max_accel_slow) 
                    accel[i] = robot_speed[i]>0? -max_accel_slow : max_accel_slow;
                else accel[i] = -robot_speed[i];
            }
            speed.z() = 0;
            // debug_tools::Debug().print("b:", static_cast<int>(path.size()), accel.x(), accel.y());
        }
        robot_speed += accel;

        // debug_tools::Debug().print("robot_speed", robot_speed.x(), robot_speed.y());

        Eigen::Vector2d pos_diff = robot_speed * control_interval_;
        robot_pos.x() += pos_diff.x();
        robot_pos.y() += pos_diff.y();

        speed.x() = (robot_speed.x()*std::cos(yaw) + robot_speed.y()*std::sin(yaw)) * 2.39;
        speed.y() = (robot_speed.x()*-std::sin(yaw) + robot_speed.y()*std::cos(yaw)) * 2.39;

        return speed;
    }

    /**
     * @brief 在订阅到全局路径后，将yaw旋转至全局路径的大致朝向
     * @param pose_traget 全局路径的大致朝向
     * @param robot_mat 机器人中心的位姿
     * @param command Eigen::Vector3d(x, y, r)
     * @return 是否满足朝向要求
     */
    bool
    updateStateSuitGlobalPose(const Eigen::Vector3d& pose_traget, const Eigen::Matrix3d& robot_mat,
        const Eigen::Vector2d& robot_speed, Eigen::Vector3d& command){
        double yaw = getYaw(robot_mat);
        auto yaw_diff = getYaw(pose_traget) - yaw;
        normYaw(yaw_diff);

        debug_tools::Debug().print("yaw_diff:", yaw_diff);

        auto slowSpeed = [&](double speed, double max_accel, double& cmd){
            if(std::abs(speed) < max_accel) cmd = 0;
            else cmd = speed>0? speed-max_accel : speed+max_accel;
        };
        Eigen::Vector2d speed;
        slowSpeed(robot_speed.x(), max_accel_, speed.x());
        slowSpeed(robot_speed.y(), max_accel_, speed.y());
        
        command.x() = (speed.x()*std::cos(yaw) + speed.y()*std::sin(yaw)) * 2.39;
        command.y() = (speed.x()*-std::sin(yaw) + speed.y()*std::cos(yaw)) * 2.39;

        if(std::abs(yaw_diff) < 0.15){
            command.z() = 0;
            return true;
        }
        command.z() = -yaw_diff;

        return false;
    }

    /**
     * @param traj_ahead_i 将path[traj_ahead_i]视为路径跟踪的目标
     * @param max_speed //x,y分方向的最大速度 v/s
     * @param max_accel //x,y分方向的最大加速度 v/s/s
     * @param c_interval //控制时间间隔 s
     */
    SpeedTracker(int traj_ahead_i, double max_speed, double max_accel, double c_interval){
        traj_ahead_index_ = traj_ahead_i;
        max_speed_ = max_speed;
        max_accel_ = max_accel*c_interval*8;
        control_interval_ = c_interval;
    }
};
} //namespace util
} //namespace motion_planning
}// namespace path_planning

#endif