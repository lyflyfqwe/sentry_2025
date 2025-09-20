cmake_minimum_required(VERSION 3.8)
project(rm_serial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rm_interface REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

find_package(debug_tools REQUIRED)
find_package(serial REQUIRED)

# find_package(simple_serial REQUIRED)

include_directories(
  include
)

add_executable(${PROJECT_NAME}_rec_command_node
  src/rec_command.cpp
)
target_link_libraries(${PROJECT_NAME}_rec_command_node
  serial
)
ament_target_dependencies(${PROJECT_NAME}_rec_command_node
  "rclcpp" 
  "std_msgs"
  "rm_interface"
  "debug_tools"
  "serial"
) 
install(TARGETS
  ${PROJECT_NAME}_rec_command_node
  DESTINATION lib/${PROJECT_NAME} 
)

# if(BUILD_TESTING)
#   add_executable(${PROJECT_NAME}_handle_datas_test
#     test/handle_datas.cpp
#   )
#   target_link_libraries(${PROJECT_NAME}_handle_datas_test
#     serial
#   )

#   add_executable(crc_test
#     test/crc_test.cpp
#   )
#   target_link_libraries(crc_test
#     serial
#   )
#   ament_target_dependencies(crc_test 
#     "rclcpp" 
#     "debug_tools"
#   )

#   add_executable(serial_reader_test
#     test/serial_reader.cpp
#   )
#   target_link_libraries(serial_reader_test
#     serial
#     simple_serial
#   )

#   add_executable(serial_reader_reconnection_test
#     test/serial_reader_reconnection.cpp
#   )
#   target_link_libraries(serial_reader_reconnection_test
#     serial
#   )

  
#   install(TARGETS 
#     ${PROJECT_NAME}_handle_datas_test
#     crc_test
#     serial_reader_test
#     serial_reader_reconnection_test
#     DESTINATION lib/${PROJECT_NAME} 
#   )
# endif()

ament_package()
