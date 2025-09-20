cmake_minimum_required(VERSION 3.8)
project(ikd_tree_mapping)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(TBB REQUIRED)
find_package(debug_tools REQUIRED)
find_package(rm_interface REQUIRED)


include_directories(
  include
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}_node
  src/ikd_tree_mapping.cpp
)

ament_target_dependencies(${PROJECT_NAME}_node
  "rclcpp"
  "nav_msgs"
  "pcl_ros"
  "sensor_msgs"
  "visualization_msgs"
  "rm_interface"
  "debug_tools"
)

add_executable(pcd_saver_node
  src/saver_pcd.cpp
)

ament_target_dependencies(pcd_saver_node
  "rclcpp"
  "rm_interface"
  "debug_tools"
)

# install executable file
install(TARGETS 
  ${PROJECT_NAME}_node
  pcd_saver_node
  DESTINATION lib/${PROJECT_NAME} 
)

ament_package()
