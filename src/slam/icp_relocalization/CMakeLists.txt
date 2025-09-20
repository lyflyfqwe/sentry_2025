cmake_minimum_required(VERSION 3.8)
project(icp_relocalization)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(debug_tools REQUIRED)
find_package(TBB REQUIRED)

include_directories(
  include
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}_node
  src/icp_relocalization.cpp
)
target_link_libraries(${PROJECT_NAME}_node
  ${PCL_LIBRARIES}
)
ament_target_dependencies(${PROJECT_NAME}_node
  "rclcpp"
  "geometry_msgs"
  "nav_msgs"
  "pcl_ros"
  "tf2_ros"
  "sensor_msgs"
  "debug_tools"
)

add_executable(${PROJECT_NAME}_sync_node
  src/icp_relocalization_sync.cpp
)
target_link_libraries(${PROJECT_NAME}_sync_node
  ${PCL_LIBRARIES}
  TBB::tbb
)
ament_target_dependencies(${PROJECT_NAME}_sync_node
  "rclcpp"
  "geometry_msgs"
  "nav_msgs"
  "pcl_ros"
  "tf2_ros"
  "sensor_msgs"
  "debug_tools"
)

add_executable(gravity_node
  src/gravity.cpp
)
ament_target_dependencies(gravity_node
  "rclcpp"
  "geometry_msgs"
  "tf2_ros"
  "sensor_msgs"
  "debug_tools"
)

add_executable(virtual_icp_node
  src/virtual_icp.cpp
)
ament_target_dependencies(virtual_icp_node
  "rclcpp"
  "geometry_msgs"
  "nav_msgs"
  "pcl_ros"
  "tf2_ros"
  "sensor_msgs"
  "debug_tools"
)

install(TARGETS 
  ${PROJECT_NAME}_node
  ${PROJECT_NAME}_sync_node
  gravity_node
  virtual_icp_node
  DESTINATION lib/${PROJECT_NAME} 
)

ament_package()
