// #include <rclcpp/rclcpp.hpp>

// #include "m_detector/dyn_obj_cluster.h"
// #include "m_detector/config.h"

// int main(int argc, char **argv){
//     rclcpp::init(argc, argv);
//     rclcpp::Node* node = new rclcpp::Node("m_detector_test_node");
//     m_detector::Config(node, "");
//     std::cout << "hello\n";
//     return 0;
// }
#include <iostream>

int main(){
    int a{0}, b{1};

    switch (a){
    if(b == 0)
    case 0:
        std::cout << "a\n";
        break;

    case 1:
        std::cout << "b\n";
        break;
    
    default:
        std::cout << "C\n";
        break;
    }

    return 0;
}