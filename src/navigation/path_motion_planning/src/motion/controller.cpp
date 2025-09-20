#include "path_motion_planning/motion/controller.h"

namespace path_motion_planning{
namespace motion{
void 
Controller::updateOdom(const Eigen::Vector3d& pos, const Eigen::Matrix3d& mat, const Eigen::Vector2d& spd){
    robot_p_ = pos;
    robot_yaw_ = Tools::getYaw(mat);
    Tools::normYaw(robot_yaw_);
    real_spd_ = spd;
}

void Controller::updatePathL(const std::vector<Eigen::Vector3d>& path){
    data_path_.setData(path);
}

void Controller::updatePathG(const std::vector<Eigen::Vector3d>& path, int traj_ahead_num){
    if(path.empty()) return;
    global_.is_new_data = true;
    auto closed_i = Tools::findClosedIndexOfPath(path, robot_p_);
    auto path_size = static_cast<int>(path.size());
    int traj_index = path_size > traj_ahead_num + closed_i? traj_ahead_num + closed_i : path_size - 1;
    global_.target_pose = path.at(traj_index) - robot_p_; //全局路径大致的朝向
    global_.target_pos = path.back();
}

Eigen::Vector3d Controller::getCommand_3d(int enforced_yaw, const Eigen::Vector3d& target_pose){
    if(global_.is_new_data){ //接受到全局路径的标志位，需要优先旋转yaw至global-path的大致朝向
        Eigen::Vector3d command;
        if(tracker_->getRobotCmdSuitGPose(global_.target_pose, robot_spd_, robot_yaw_, command)) global_.is_new_data = false;
        return command;
    }

    if(enforced_yaw == 2){
        Eigen::Vector3d command;
        tracker_->getRobotCmdSuitGPose(target_pose, robot_spd_, robot_yaw_, command);
        return command;
    }
    data_path_.lock(true);
    auto path = data_path_.getData();
    data_path_.lock(false);

    if(enforced_yaw == 1){
        tracker_->setTrackPose(target_pose);
        return tracker_->getRobotCmd(path, global_.target_pos, robot_p_, robot_spd_, robot_yaw_, true);
    }
    else return tracker_->getRobotCmd(path, global_.target_pos, robot_p_, robot_spd_, robot_yaw_, false);
}

Eigen::Vector<double, 5> Controller::getCommand_5d(int enforced_yaw, const Eigen::Vector3d& target_pose){
    if(global_.is_new_data){ //接受到全局路径的标志位，需要优先旋转yaw至global-path的大致朝向
        Eigen::Vector3d command;
        if(tracker_->getRobotCmdSuitGPose(global_.target_pose, robot_spd_, robot_yaw_, command)) global_.is_new_data = false;
        return Eigen::Vector<double, 5>{command[0], command[1], command[2], robot_spd_.x(), robot_spd_.y()};
    }

    // if(enforced_yaw){
    //     Eigen::Vector3d command;
    //     tracker_->getRobotCmdSuitGPose(target_pose, robot_spd_, robot_yaw_, command);
    //     return Eigen::Vector<double, 5>{command[0], command[1], command[2], robot_spd_.x(), robot_spd_.y()};
    // }

    // data_path_.lock(true);
    // auto path = data_path_.getData();
    // data_path_.lock(false);

    // auto cmd = tracker_->getRobotCmd(path, global_.target_pos, robot_p_, robot_spd_, robot_yaw_);
    // return Eigen::Vector<double, 5>{cmd.x(), cmd.y(), cmd.z(), robot_spd_.x(), robot_spd_.y()};

    if(enforced_yaw == 2){
        Eigen::Vector3d command;
        tracker_->getRobotCmdSuitGPose(target_pose, robot_spd_, robot_yaw_, command);
        return Eigen::Vector<double, 5>{command[0], command[1], command[2], robot_spd_.x(), robot_spd_.y()};
    }
    data_path_.lock(true);
    auto path = data_path_.getData();
    data_path_.lock(false);

    Eigen::Vector3d cmd;
    if(enforced_yaw == 1){
        tracker_->setTrackPose(target_pose);
        cmd = tracker_->getRobotCmd(path, global_.target_pos, robot_p_, robot_spd_, robot_yaw_, true);
    }
    else cmd = tracker_->getRobotCmd(path, global_.target_pos, robot_p_, robot_spd_, robot_yaw_, false);

    return Eigen::Vector<double, 5>{cmd.x(), cmd.y(), cmd.z(), robot_spd_.x(), robot_spd_.y()};
}

} //namespace motion
} // namespace path_motion_planning