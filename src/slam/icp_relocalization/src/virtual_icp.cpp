#include <Eigen/Eigen>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <nav_msgs/msg/odometry.hpp>

namespace icp_relocalization{
class VirtualICP: public rclcpp::Node{
private:    
    rclcpp::TimerBase::SharedPtr timer_send_tf_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_odom_;

    void sendTF(){
        Eigen::Vector3d trans = Eigen::Vector3d::Zero(); 
        auto rot = Eigen::Quaterniond(Eigen::Matrix3d::Identity());

        nav_msgs::msg::Odometry msg_odom;
        msg_odom.header.stamp = this->now();
        msg_odom.header.frame_id = "map";
        msg_odom.child_frame_id = "camera_init";

        msg_odom.pose.pose.position.x = trans.x();
        msg_odom.pose.pose.position.y = trans.y();
        msg_odom.pose.pose.position.z = trans.z();
        msg_odom.pose.pose.orientation.x = rot.x();
        msg_odom.pose.pose.orientation.y = rot.y();
        msg_odom.pose.pose.orientation.z = rot.z();
        msg_odom.pose.pose.orientation.w = rot.w();

        pub_odom_->publish(msg_odom);

        geometry_msgs::msg::TransformStamped msg_tf;
        msg_tf.header.stamp = this->now();
        msg_tf.header.frame_id = "map";
        msg_tf.child_frame_id = "camera_init";

        msg_tf.transform.translation.x = trans.x();
        msg_tf.transform.translation.y = trans.y();
        msg_tf.transform.translation.z = trans.z();
        msg_tf.transform.rotation.x = rot.x();
        msg_tf.transform.rotation.y = rot.y();
        msg_tf.transform.rotation.z = rot.z();
        msg_tf.transform.rotation.w = rot.w();

        tf_broadcaster_->sendTransform(msg_tf);
    }

public: 
    VirtualICP(): Node("virtual_icp_node", rclcpp::NodeOptions()){
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(this);

        std::string topic_odom_icp = declare_parameter<std::string>("topic_odom_icp", "/icp_odom");
        pub_odom_ = this->create_publisher<nav_msgs::msg::Odometry>(topic_odom_icp, 10);

        timer_send_tf_ = this->create_wall_timer(
            std::chrono::milliseconds(20), std::bind(&VirtualICP::sendTF, this));
    }
};
} //namespace icp_relocalization

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto icp_node = std::make_shared<icp_relocalization::VirtualICP>();
    rclcpp::spin(icp_node);
    rclcpp::shutdown();
    return 0;
}