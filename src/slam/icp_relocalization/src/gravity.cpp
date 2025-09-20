#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <sensor_msgs/msg/imu.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <Eigen/Dense>

#include "debug_tools/debug_tools.h"

namespace icp_relocalization{
class Gravity: public rclcpp::Node{
private:
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_imu_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    // 计算从给定加速度向量到重力方向的旋转矩阵
    inline Eigen::Matrix3d calRotationMatrix(Eigen::Vector3d& imu_acc){
        auto g_acc = Eigen::Vector3d(0, 0, -1);
        auto v = imu_acc.cross(g_acc); 
        double theta = std::acos(imu_acc.dot(g_acc));
        Eigen::Matrix3d K;  
        K <<  0, -v.z(), v.y(),  
            v.z(), 0, -v.x(),  
            -v.y(), v.x(), 0;  
        Eigen::Matrix3d R = Eigen::Matrix3d::Identity() + sin(theta) * K + (1 - cos(theta)) * K * K; 
        return R;
    }

    Eigen::Matrix3d compute_init_g(const Eigen::Vector3d& a) {  
        // 三轴加速度  
        Eigen::Vector3d g = a.normalized();  // 归一化加速度作为重力方向  

        // 计算与 g 正交的任意向量  
        Eigen::Vector3d arbitrary_vector(1.0, 0.0, 0.0); // 假设初始的任意向量为X轴  
        if (fabs(g[0]) > 0.999) {  
            arbitrary_vector = Eigen::Vector3d(0.0, 0.0, 1.0); // 如果 g 接近 X 轴，使用 Y 轴  
        }  

        // 计算 X 轴  
        Eigen::Vector3d x_init_g = g.cross(arbitrary_vector);  
        x_init_g.normalize();  // 归一化  

        // 计算 Y 轴  
        Eigen::Vector3d y_init_g = g.cross(x_init_g);  

        // 构建旋转矩阵 R  
        Eigen::Matrix3d R;  
        R.col(0) = x_init_g;  
        R.col(1) = y_init_g;  
        R.col(2) = g;  // Z 轴是 g 方向  

        return R;  
    }  

    Eigen::Matrix3d compute_euler_angles(const Eigen::Vector3d& accel, const Eigen::Vector3d& gyro, double dt) {  
        // 提取加速度向量  
        double acc_x = accel[0];  
        double acc_y = accel[1];  
        double acc_z = accel[2];  

        // 计算 Roll 和 Pitch  
        double roll = atan2(acc_y, acc_z); // Roll  
        double pitch = atan2(-acc_x, sqrt(acc_y * acc_y + acc_z * acc_z)); // Pitch  

        // 如果有角速度（陀螺仪），可以使用互补滤波来融合  
        static double gyro_x = 0.0;  
        static double gyro_y = 0.0;  
        static double gyro_z = 0.0;  

        // 使用角速度更新欧拉角（简单的互补滤波）  
        gyro_x += gyro[0] * dt;  
        gyro_y += gyro[1] * dt;  
        gyro_z += gyro[2] * dt;  

        // 将径向旋转（Yaw）设为 0，这里可以根据需要进行调整  
        double yaw = 0.0; // 在不同情况下，可以级联计算 Yaw  

        // 以 ZYX 顺序计算总旋转矩阵  
        Eigen::Matrix3d Rz, Ry, Rx;  

        Rz << cos(yaw), -sin(yaw), 0,  
              sin(yaw), cos(yaw), 0,  
              0, 0, 1;  

        Ry << cos(pitch), 0, sin(pitch),  
              0, 1, 0,  
              -sin(pitch), 0, cos(pitch);  

        Rx << 1, 0, 0,  
              0, cos(roll), -sin(roll),  
              0, sin(roll), cos(roll);  

        // 返回兀旋转矩阵  
        return Rz * Ry * Rx;  
    }  

    // 定义将当前 odom 转换到新 odom 的函数  
    void transform_odom(const Eigen::Vector3d& current_position, const Eigen::Quaterniond& current_orientation,   
                        const Eigen::Matrix3d& rotation_matrix,   
                        Eigen::Vector3d& new_position, Eigen::Quaterniond& new_orientation) {  
                        
        // 转换位置  
        new_position = rotation_matrix * current_position;  

        // 计算新的四元数  
        Eigen::Matrix3d current_rotation = current_orientation.toRotationMatrix();  
        Eigen::Matrix3d new_rotation = rotation_matrix * current_rotation;  
        new_orientation = Eigen::Quaterniond(new_rotation);  
    }

    void imuCallBack(const sensor_msgs::msg::Imu::SharedPtr msg){
        debug_tools::Timer t_begin;
        auto imu_acc = Eigen::Vector3d(msg->linear_acceleration.x, msg->linear_acceleration.y, msg->linear_acceleration.z).normalized();
        auto rot_matrix = calRotationMatrix(imu_acc);
        Eigen::Quaternion<double> rot(rot_matrix);
        geometry_msgs::msg::TransformStamped msg_tf;
        msg_tf.header.stamp = msg->header.stamp;
        msg_tf.header.frame_id = "body";
        msg_tf.child_frame_id = "gravity";
        msg_tf.transform.translation.x = 0;
        msg_tf.transform.translation.y = 0;
        msg_tf.transform.translation.z = 0;
        msg_tf.transform.rotation.x = rot.x();
        msg_tf.transform.rotation.y = rot.y();
        msg_tf.transform.rotation.z = rot.z();
        msg_tf.transform.rotation.w = rot.w();
        tf_broadcaster_->sendTransform(msg_tf);
    }

public:
    Gravity(): Node("Gravity_node", rclcpp::NodeOptions()){
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);
        sub_imu_ = this->create_subscription<sensor_msgs::msg::Imu>("/livox/imu", 10, std::bind(&Gravity::imuCallBack, this, std::placeholders::_1));
    }
};
}

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto gravity_node = std::make_shared<icp_relocalization::Gravity>();
    rclcpp::spin(gravity_node);
    rclcpp::shutdown();
    return 0;
}