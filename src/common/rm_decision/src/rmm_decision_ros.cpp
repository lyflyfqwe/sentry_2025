#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include "rm_interface/srv/nav_core.hpp"
#include "rm_interface/msg/local_state.hpp"
#include "rm_interface/msg/enforced_yaw.hpp"
#include "rm_interface/srv/set_robot_mode.hpp"

#include "rm_decision/tools.h"
#include "rm_decision/vis.h"
#include "rm_decision/rmm_decision.h"

namespace rm_decision{
class RMMDecisionROS: public rclcpp::Node, public Tools, public Visual{
private:
    struct Interface{
        rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_enemy;
        rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub_sentry_info;
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_odom;
        rclcpp::Subscription<rm_interface::msg::LocalState>::SharedPtr sub_local_state;
        rclcpp::Client<rm_interface::srv::NavCore>::SharedPtr client_nav;
        rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr pub_enemy_vis;
        rclcpp::Client<rm_interface::srv::SetRobotMode>::SharedPtr client_speed;
        rclcpp::Publisher<rm_interface::msg::EnforcedYaw>::SharedPtr yaw_pub;
    }itf_;

    struct RobotOdom{
        Eigen::Vector3d pos;
        Eigen::Matrix3d mat;
    }odom_; //车体中心的里程计 map->robot

    struct EnemyOffSet{
        Eigen::Vector3d pos;
        Eigen::Quaterniond qua;
    }e_offset_; //自瞄坐标系与车体中心坐标系的偏移
    
    RMMDecision::Ptr decision_;
    std::thread thd_runner_, thd_enforced_yaw_;

    std::mutex mutex_;

protected:
    /**
     * @brief 订阅敌人坐标的回调
     */
    inline void callBackEnemy(const std_msgs::msg::Float32MultiArray::ConstSharedPtr& msg){
        if(msg->data.size() != 4){
            debug_tools::Debug().print("callBackEnemy error");
            return;
        }
        auto pos_origin = Eigen::Vector3d{msg->data.at(0), msg->data.at(1), msg->data.at(2)};
        Eigen::Vector3d pos_world = odom_.mat * (e_offset_.qua * pos_origin + e_offset_.pos) + odom_.pos;
        auto value = msg->data.at(3);
        if(value > 0){
            Eigen::Vector3d pos_world_ey = odom_.mat * ( pos_origin + e_offset_.pos) + odom_.pos;
            decision_->updateAim(pos_world, pos_world_ey - odom_.pos, value);
            Visual::pubCubeVisual(itf_.pub_enemy_vis, this, {pos_world}, {0.1, 0.1, 0.1}, {0.0f, 1.0f, 1.0f});
        }
        else{
            decision_->updateAim(pos_world, Eigen::Vector3d::Zero(), 0);
            Visual::pubCubeVisual(itf_.pub_enemy_vis, this, {}, {0.1, 0.1, 0.1}, {0.0f, 1.0f, 1.0f});
        } 
    }

    /**
     * @brief 订阅哨兵自身信息的回调
     */
    inline void callBackSentryInfo(const std_msgs::msg::Int32MultiArray::ConstSharedPtr& msg){
        decision_->updateSentryInfo(msg->data);
    }

    inline void callBackOdom(const nav_msgs::msg::Odometry::ConstSharedPtr& msg){
        odom_.pos = Eigen::Vector3d(msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z);
        odom_.mat = Eigen::Quaterniond(msg->pose.pose.orientation.w, msg->pose.pose.orientation.x, 
                msg->pose.pose.orientation.y, msg->pose.pose.orientation.z).matrix();
        decision_->updateOdom(odom_.pos, getYaw(odom_.mat));
    }

    inline void callBackState(const rm_interface::msg::LocalState::ConstSharedPtr& msg){
        decision_->updateLFailNum(msg->result);
        if(msg->safe_state){
             if(itf_.client_speed->service_is_ready()){
                double speed = decision_->changeSpeed();
                auto req = std::make_shared<rm_interface::srv::SetRobotMode::Request>();
                req->robot_speed_rate = speed;
                itf_.client_speed->async_send_request(req);
             }
         }
    }

    /**
     * @brief 尝试请求一次导航服务
     */
    inline void tryReqServer(const Eigen::Vector3d& pos, bool is_track_mode){
        std::lock_guard<std::mutex> lock(mutex_);
        if(!itf_.client_nav->service_is_ready()){
            debug_tools::Debug().print("waiting for service to appear...");
            decision_->callBackNavReqResult(false, is_track_mode);
            return;
        } 

        debug_tools::Debug().print("tryReqServer: [", pos.x(), pos.y(), pos.z(), "]");

        auto req = std::make_shared<rm_interface::srv::NavCore::Request>();
        auto resp = std::make_shared<rm_interface::srv::NavCore::Response>();
        req->x = pos.x(); req->y = pos.y(); req->z = pos.z(); 
        itf_.client_nav->async_send_request(req, 
            [this, &resp, &is_track_mode](rclcpp::Client<rm_interface::srv::NavCore>::SharedFuture cb){ 
                bool result{false};
                try{
                    result = (cb.get()->result == 0);
                }catch (const std::exception &e){} 
                decision_->callBackNavReqResult(result, is_track_mode);
        });
    }

    inline void callBackEnforcedYaw(){
        while(rclcpp::ok()){
            rm_interface::msg::EnforcedYaw msg_ef;
            Eigen::Vector3d pose;
            decision_->getEFYawCommand(msg_ef.flag, pose, msg_ef.spin, msg_ef.ahead);
            msg_ef.x = pose.x();
            msg_ef.y = pose.y();
            msg_ef.z = pose.z();
            itf_.yaw_pub->publish(msg_ef);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    inline void callBackDecision(){
        Eigen::Vector3d goal;
        while(rclcpp::ok()){
            bool is_track_mode;
            if(decision_->getTargetPos(goal, is_track_mode)) tryReqServer(goal, is_track_mode);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

public:
    RMMDecisionROS(): Node("rm_decision_node", rclcpp::NodeOptions()){
        decision_ = std::make_shared<RMMDecision>();

        auto e_offset_pos = declare_parameter<std::vector<double>>("decision.enemy_offset.pos", {0, -0.1, 0.1});
        Tools::vecToEigen(e_offset_pos, e_offset_.pos);

        auto e_offset_eul = declare_parameter<std::vector<double>>("decision.enemy_offset.eul", {0, 0, -20.0}); //角度制 roll pitch yaw
        e_offset_.qua = Tools::eulerToQuaternion(
                e_offset_eul.at(0)/180.0*M_PI, e_offset_eul.at(1)/180.0*M_PI, e_offset_eul.at(2)/180.0*M_PI );

        odom_.mat = Eigen::Quaterniond(1.0, 0, 0, 0).matrix();
        odom_.pos = Eigen::Vector3d(0, 0, 0);

        Visual::loadParamToRegion<double>("decision.zone.first", decision_->regions_[Regions::FIRST], this,{
            1.52, -4.62, -0.06,
     1.18, -3.52, -0.06,
      2.26, -2.10, -0.06,
       3.14, -3.50, -0.07
     });
     auto points_zone_f = decision_->regions_[Regions::FIRST].getPoints();

        Visual::loadParamToRegion<double>("decision.zone.second", decision_->regions_[Regions::SECOND], this,{
            2.14, 2.51, -0.07,
     2.40, 1.80, -0.04,
      2.10, 1.06, -0.07,
       1.53, 2.62, -0.07
     });
     auto points_zone_s = decision_->regions_[Regions::SECOND].getPoints();

        Visual::loadParamToRegion<double>("decision.zone.third", decision_->regions_[Regions::THIRD], this,{
            2.14, 2.51, -0.07,
     2.40, 1.80, -0.04,
      2.10, 1.06, -0.07,
       1.53, 2.62, -0.07
     });
     auto points_zone_t = decision_->regions_[Regions::THIRD].getPoints();

        Visual::loadParamToRegion<double>("decision.region.uphill", decision_->regions_[Regions::UPHILL], this,{
        5.35,4.06,-0.42,
        5.22,3.52,-0.42,
        5.79,3.61,-0.42,
        5.72,4.05,-0.42,
     });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_uphill = decision_->regions_[Regions::UPHILL].getPoints();
        for(const auto& p : points_uphill) debug_tools::Debug().print(p.x(), p.y());
        

        Visual::loadParamToRegion<double>("decision.region.cure", decision_->regions_[Regions::CURE], this,{
            6.45,-3.22,-0.79,
            6.95,-4.48,-0.79,
            6.19,-4.76,-0.79,
            5.69,-3.82,-0.79
     });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_cure = decision_->regions_[Regions::CURE].getPoints();
        for(const auto& p : points_cure) debug_tools::Debug().print(p.x(), p.y());

        Visual::loadParamToRegion<double>("decision.region.guard", decision_->regions_[Regions::GUARD], this,{
            5.12,0.73,-0.80,
            5.45,-0.30,-0.80,
            4.56,-0.46,-0.80,
            4.17,0.36,-0.84
        });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_guard = decision_->regions_[Regions::GUARD].getPoints();
        for(const auto& p : points_guard) debug_tools::Debug().print(p.x(), p.y());

        Visual::loadParamToRegion<double>("decision.region.guards", decision_->regions_[Regions::GUARDS], this,{
            5.12,0.73,-0.80,
            5.45,-0.30,-0.80,
            4.56,-0.46,-0.80,
            4.17,0.36,-0.84
       });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_guards = decision_->regions_[Regions::GUARDS].getPoints();
        for(const auto& p : points_guards) debug_tools::Debug().print(p.x(), p.y());

        Visual::loadParamToRegion<double>("decision.region.guardt", decision_->regions_[Regions::GUARDT], this,{
            5.12,0.73,-0.80,
            5.45,-0.30,-0.80,
            4.56,-0.46,-0.80,
            4.17,0.36,-0.84
       });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_guardt = decision_->regions_[Regions::GUARDT].getPoints();
        for(const auto& p : points_guardt) debug_tools::Debug().print(p.x(), p.y());

        Visual::loadParamToRegion<double>("decision.region.tower", decision_->regions_[Regions::TOWER], this,{
            6.45,-3.22,-0.79,
            6.95,-4.48,-0.79,
            6.19,-4.76,-0.79,
            5.69,-3.82,-0.79
       });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_tower = decision_->regions_[Regions::TOWER].getPoints();
        for(const auto& p : points_tower) debug_tools::Debug().print(p.x(), p.y());

        Visual::loadParamToRegion<double>("decision.region.place", decision_->regions_[Regions::PLACE], this,{
            2.14, 2.51, -0.07,
     2.40, 1.80, -0.04,
      2.10, 1.06, -0.07,
       1.53, 2.62, -0.07
       });
        debug_tools::Debug().print("regions size:", static_cast<int>(decision_->regions_.size()));
        auto points_place = decision_->regions_[Regions::PLACE].getPoints();
        for(const auto& p : points_place) debug_tools::Debug().print(p.x(), p.y());

        std::string topic_pub_enemy = declare_parameter<std::string>("decision.topic_pub_enemy_cube", "/vis_enemy_cube");
        itf_.pub_enemy_vis = this->create_publisher<visualization_msgs::msg::MarkerArray>(topic_pub_enemy, 10);
    
        std::string topic_sub_enemy = declare_parameter<std::string>("decision.topic_sub_enemy", "/sentry/armor_solver/nav_pose");
        itf_.sub_enemy = this->create_subscription<std_msgs::msg::Float32MultiArray>(
                topic_sub_enemy, rclcpp::SensorDataQoS(), std::bind(&RMMDecisionROS::callBackEnemy, this, std::placeholders::_1));

        std::string topic_sub_sentry = declare_parameter<std::string>("decision.topic_sub_sentry", "/rm_sentry_info");
        itf_.sub_sentry_info = this->create_subscription<std_msgs::msg::Int32MultiArray>(
                topic_sub_sentry, 10, std::bind(&RMMDecisionROS::callBackSentryInfo, this, std::placeholders::_1));

        std::string topic_sub_odom = declare_parameter<std::string>("decision.topic_sub_odom", "/nav_core_odom");
        itf_.sub_odom = create_subscription<nav_msgs::msg::Odometry>(topic_sub_odom, rclcpp::QoS(1).best_effort(),
                std::bind(&RMMDecisionROS::callBackOdom, this, std::placeholders::_1));

        std::string topic_sub_state =declare_parameter<std::string>("decision.topic_sub_state","/nav_core_local_state");
        itf_.sub_local_state = this->create_subscription<rm_interface::msg::LocalState>(
            topic_sub_state,10,std::bind(&RMMDecisionROS::callBackState,this,std::placeholders::_1));  

        std::string topic_nav_service = declare_parameter<std::string>("plan.service_nav", "/rm_nav");
        itf_.client_nav = this->create_client<rm_interface::srv::NavCore>(topic_nav_service);

        std::string topic_nav_speed = declare_parameter<std::string>("plan.speed","/rm_speed");
        itf_.client_speed =this->create_client<rm_interface::srv::SetRobotMode>(topic_nav_speed);

        std::string topic_pub_yaw = declare_parameter<std::string>("decision.topic_yaw","/enforced_yaw");
        itf_.yaw_pub = this->create_publisher<rm_interface::msg::EnforcedYaw>(topic_pub_yaw,10);

        thd_runner_ = std::thread(&RMMDecisionROS::callBackDecision, this);

        thd_enforced_yaw_ = std::thread(&RMMDecisionROS::callBackEnforcedYaw, this);

        decision_->run();
    }

    ~RMMDecisionROS(){
        decision_->stop();
        if(thd_runner_.joinable()) thd_runner_.join();
        if(thd_enforced_yaw_.joinable()) thd_enforced_yaw_.join();
        debug_tools::Debug().print("~RMMDecisionROS");
    }
}; //class RMMDecisionROS
} //namespace rm_decision

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rm_decision::RMMDecisionROS>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}