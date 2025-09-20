cmake_minimum_required(VERSION 3.8)
project(tilted_lidar)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(tf2_ros REQUIRED)

find_package(Eigen3 REQUIRED)

include_directories(
  include
  SYSTEM
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}_node
  src/tilted_lidar.cpp
)

target_link_libraries(${PROJECT_NAME}_node
  ${PCL_LIBRARIES}
)

ament_target_dependencies(${PROJECT_NAME}_node
  "rclcpp"
  "pcl_ros"
  "sensor_msgs"
  "pcl_conversions"
)

add_executable(publish_body_cloud_node
  src/publish_body_cloud.cpp
)

target_link_libraries(publish_body_cloud_node
  ${PCL_LIBRARIES}
)

ament_target_dependencies(publish_body_cloud_node
  "rclcpp"
  "pcl_ros"
  "sensor_msgs"
  "pcl_conversions"
)

add_executable(pointcloud_icp_node
  src/icp_demo.cpp)  

target_link_libraries(pointcloud_icp_node
  ${PCL_LIBRARIES}
)

ament_target_dependencies(pointcloud_icp_node  
  "rclcpp"  
  "sensor_msgs"
  "pcl_conversions"
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS 
  ${PROJECT_NAME}_node
  publish_body_cloud_node
  pointcloud_icp_node
  DESTINATION lib/${PROJECT_NAME} 
)

install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
