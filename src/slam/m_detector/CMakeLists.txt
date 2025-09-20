cmake_minimum_required(VERSION 3.8)
project(m_detector)

add_compile_options(-std=c++17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_FLAGS "-fms-extensions")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(livox_ros_driver2 REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(tf2_ros REQUIRED)

find_package(pcl_conversions REQUIRED)
find_package(Eigen3 REQUIRED)
# find_package(PCL REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)

include_directories(
  include
  SYSTEM  #添加以解决包含头文件<pcl/point_types.h>报错
  ${OpenCV_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} SHARED
  src/dyn_obj_cluster.cpp
)

target_link_libraries(${PROJECT_NAME} 
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES}
  fmt::fmt
  TBB::tbb
)

# add_executable(${PROJECT_NAME}_node
#   src/dyn_obj_filter.cpp
#   src/main_node.cpp
# )

add_executable(${PROJECT_NAME}_node
  src/dyn_obj_filter.cpp
  src/main_node.cpp
  src/dyn_obj_cluster.cpp
)

target_link_libraries(${PROJECT_NAME}_node
  ${PROJECT_NAME}
)

# target_link_libraries(${PROJECT_NAME}_node
#   ${OpenCV_LIBS}
#   ${PCL_LIBRARIES}
#   fmt::fmt
#   TBB::tbb
# )

ament_target_dependencies(${PROJECT_NAME}_node
  "rclcpp"
  "rclpy"
  "geometry_msgs"
  "nav_msgs"
  "sensor_msgs"
  "visualization_msgs"
  "std_msgs"
  "pcl_ros"
  "tf2_ros"

  "livox_ros_driver2"
)

add_executable(${PROJECT_NAME}_node_test
  test/main_test.cpp
)

target_link_libraries(${PROJECT_NAME}_node_test
  ${PROJECT_NAME}
)

ament_target_dependencies(${PROJECT_NAME}_node_test
  "rclcpp"
  "rclpy"
  "geometry_msgs"
  "nav_msgs"
  "sensor_msgs"
  "visualization_msgs"
  "std_msgs"
  "pcl_ros"
  "tf2_ros"
  "livox_ros_driver2"
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

# install lib file
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# install executable file
install(TARGETS 
  ${PROJECT_NAME}_node_test 
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME} 
)

# Install config and launch files.
# install(DIRECTORY launch
#   DESTINATION share/${PROJECT_NAME}
# )

ament_package()
