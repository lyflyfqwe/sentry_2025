// pcd_visualizer_node.cpp

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <std_srvs/srv/empty.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/common/common.h>

#include <random>
#include <string>

class PCDVisualizer : public rclcpp::Node {
public:
    PCDVisualizer()
    : Node("pcd_visualizer") {
        cloud_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("cloud", 10);

        clicked_sub_ = this->create_subscription<geometry_msgs::msg::PointStamped>(
            "/clicked_point", 10,
            std::bind(&PCDVisualizer::onClickedPoint, this, std::placeholders::_1)
        );

        save_srv_ = this->create_service<std_srvs::srv::Empty>(
            "save_cloud",
            std::bind(&PCDVisualizer::onSaveRequest, this, std::placeholders::_1, std::placeholders::_2)
        );

        // 加载原始点云
        pcl::io::loadPCDFile("/home/ld/Documents/point_cloud/inf.pcd", *cloud_);
        publishCloud();
    }

private:
    void publishCloud() {
        sensor_msgs::msg::PointCloud2 msg;
        pcl::toROSMsg(*cloud_, msg);
        msg.header.frame_id = "map";
        cloud_pub_->publish(msg);
    }

    void onClickedPoint(const geometry_msgs::msg::PointStamped::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Clicked point: [%.2f, %.2f, %.2f]", msg->point.x, msg->point.y, msg->point.z);

        // 在该点周围生成一个立方体体积的随机点
        float cube_size = 0.5;  // 立方体边长
        int num_points = 50000;

        std::default_random_engine rng;
        std::uniform_real_distribution<float> dist(-cube_size / 2, cube_size / 2);

        for (int i = 0; i < num_points; ++i) {
            pcl::PointXYZ p;
            p.x = msg->point.x + dist(rng);
            p.y = msg->point.y + dist(rng);
            p.z = msg->point.z + dist(rng);
            cloud_->points.push_back(p);
        }

        cloud_->width = cloud_->points.size();
        cloud_->height = 1;
        cloud_->is_dense = true;

        publishCloud();
    }

    void onSaveRequest(
        const std::shared_ptr<std_srvs::srv::Empty::Request>,
        std::shared_ptr<std_srvs::srv::Empty::Response>) {
        
        std::string out_file = "/home/ld/Documents/point_cloud/inf_2.pcd";
        pcl::io::savePCDFileBinary(out_file, *cloud_);
        RCLCPP_INFO(this->get_logger(), "Saved cloud to %s", out_file.c_str());
    }

    // ROS
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr cloud_pub_;
    rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr clicked_sub_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr save_srv_;

    // Point cloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_ { new pcl::PointCloud<pcl::PointXYZ>() };
};

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PCDVisualizer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}