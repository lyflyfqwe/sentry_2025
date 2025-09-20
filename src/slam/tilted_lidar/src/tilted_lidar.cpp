#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>  

#include "tilted_lidar/random_cloud.h"
#include "tilted_lidar/debug.h"

#include <Eigen/Eigen>

namespace tilted_lidar{

class TiltedLidar: public rclcpp::Node{
private:
    Eigen::Vector3f T_b_{-0.5, -0.4, -0.7}; //位置偏移 [x, y, z]
    Eigen::Vector3f R_b_{0.585, 0, 0};  //欧拉角 姿态偏移 [roll绕x轴  pitch绕y轴  yaw绕z轴]
    Eigen::Matrix3f R_inverse; //R_b_的旋转矩阵 的逆矩阵

    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcast_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_cloud_vbody_;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_cloud_body;

    inline geometry_msgs::msg::Quaternion eigenToGeometryQuaternion(const Eigen::Vector3f &euler_angles){  
        // 计算旋转的四元数  
        Eigen::AngleAxisd roll(euler_angles.x(), Eigen::Vector3d::UnitX());  
        Eigen::AngleAxisd pitch(euler_angles.y(), Eigen::Vector3d::UnitY());  
        Eigen::AngleAxisd yaw(euler_angles.z(), Eigen::Vector3d::UnitZ());  

        // 按照 ZYX 顺序组合旋转  
        Eigen::Quaterniond q = yaw * pitch * roll;  
 
        geometry_msgs::msg::Quaternion ros_quaternion;  
        ros_quaternion.x = q.x();  
        ros_quaternion.y = q.y();  
        ros_quaternion.z = q.z();  
        ros_quaternion.w = q.w();  

        return ros_quaternion;  
    }

public:
    TiltedLidar(std::string node_name): Node(node_name){
        tf_broadcast_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        pub_cloud_vbody_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/cloud_v_body", 10);

        Eigen::Matrix3f R;
        R = Eigen::AngleAxisf(R_b_(2), Eigen::Vector3f::UnitZ())  
                        * Eigen::AngleAxisf(R_b_(1), Eigen::Vector3f::UnitY())  
                        * Eigen::AngleAxisf(R_b_(0), Eigen::Vector3f::UnitX());
        R_inverse = R.inverse();

        timer_ = this->create_wall_timer(std::chrono::milliseconds(50), [this](){
            geometry_msgs::msg::TransformStamped msg_tf_body_vbody;
            msg_tf_body_vbody.header.stamp = this->now();
            msg_tf_body_vbody.header.frame_id = "body";
            msg_tf_body_vbody.child_frame_id = "v_body";

            msg_tf_body_vbody.transform.translation.x = T_b_(0);
            msg_tf_body_vbody.transform.translation.y = T_b_(1);
            msg_tf_body_vbody.transform.translation.z = T_b_(2);

            msg_tf_body_vbody.transform.rotation = eigenToGeometryQuaternion(R_b_);

            tf_broadcast_->sendTransform(msg_tf_body_vbody);
        });

        sub_cloud_body = this->create_subscription<sensor_msgs::msg::PointCloud2>("/cloud_body", 10, [this](const sensor_msgs::msg::PointCloud2::SharedPtr msg){
            auto cloud_in = std::make_shared<pcl::PointCloud<pcl::PointXYZ>>();
            pcl::fromROSMsg(*msg, *cloud_in);
            Eigen::Vector3f point_vec;
            for(auto& p : cloud_in->points){
                point_vec(0) = p.x, point_vec(1) = p.y, point_vec(2) = p.z;
                point_vec = R_inverse * (point_vec - T_b_);
                p.x = point_vec(0), p.y = point_vec(1), p.z = point_vec(2);
            }
            sensor_msgs::msg::PointCloud2 msg_cloud_out;
            pcl::toROSMsg(*cloud_in, msg_cloud_out);
            msg_cloud_out.header.frame_id = "v_body";
            msg_cloud_out.header.stamp = msg->header.stamp;

            pub_cloud_vbody_->publish(msg_cloud_out);
        });
    }

};

}
// 假设body是雷达的frame， v_body是车体中心的frame

// 上方的两个参数T_b_, R_b_分别对应 body与v_body的位置偏移[x,y,z] 和姿态偏移[roll绕x轴  pitch绕y轴  yaw绕z轴]
// 循环发布body和v_body的tf关系
// 订阅点云话题/cloud_body, 话题点云的frame是body
// 已知body和v_body的关系, 遍历被订阅点云的每一个点,转换为v_body坐标系,最后获得frame为v_body的点云, 再通过话题/cloud_v_body发布出去,在rviz上呈现彩色
// 两点云的frame不同，但位置及姿态相同

// 导航输入: 车体的frame:v_body    车体观测的点云:/cloud_v_body   车体的里程计: 与点云转换坐标系的方法类似，单数据结构不同，就不写了

int main(int argc, char** argv){
    // rclcpp::init(argc, argv);  
    // rclcpp::spin(std::make_shared<tilted_lidar::TiltedLidar>("tilted_lidar_node"));  
    // rclcpp::shutdown();  

    debug::Debug().print("Example", 42, 3.14, "Hello", std::string("World"), true);
    
    return 0;  
}