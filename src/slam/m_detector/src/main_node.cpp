#include "m_detector/dyn_obj_filter.h"

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto m_detector_node = 
        std::make_shared<m_detector::DynObjFilter>("m_detector_node", rclcpp::NodeOptions());
    rclcpp::spin(m_detector_node);
    rclcpp::shutdown();
    return 0;
}