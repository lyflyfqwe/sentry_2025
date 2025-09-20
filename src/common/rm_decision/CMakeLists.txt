cmake_minimum_required(VERSION 3.8)
project(rm_decision)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(debug_tools REQUIRED)
find_package(rm_interface REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
)

add_executable(rm_decision_node 
  src/rmm_decision.cpp
  src/rmm_decision_ros.cpp
)

ament_target_dependencies(rm_decision_node
  "rclcpp"
  "std_msgs"
  "nav_msgs"
  "visualization_msgs"
  "debug_tools"
  "rm_interface"
)

install(TARGETS 
  rm_decision_node
  DESTINATION lib/${PROJECT_NAME} 
)

if(BUILD_TESTING)
  add_executable(test_node 
    test/test.cpp
    src/rm_decision.cpp
  )
  ament_target_dependencies(test_node
    "rclcpp"
    "debug_tools"
  )

  install(TARGETS 
    test_node
    DESTINATION lib/${PROJECT_NAME} 
  )
endif()



ament_package()
