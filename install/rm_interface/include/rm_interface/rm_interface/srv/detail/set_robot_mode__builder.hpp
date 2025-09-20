// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:srv/SetRobotMode.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__BUILDER_HPP_
#define RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/srv/detail/set_robot_mode__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_SetRobotMode_Request_robot_speed_rate
{
public:
  Init_SetRobotMode_Request_robot_speed_rate()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rm_interface::srv::SetRobotMode_Request robot_speed_rate(::rm_interface::srv::SetRobotMode_Request::_robot_speed_rate_type arg)
  {
    msg_.robot_speed_rate = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::SetRobotMode_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::SetRobotMode_Request>()
{
  return rm_interface::srv::builder::Init_SetRobotMode_Request_robot_speed_rate();
}

}  // namespace rm_interface


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_SetRobotMode_Response_result
{
public:
  Init_SetRobotMode_Response_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rm_interface::srv::SetRobotMode_Response result(::rm_interface::srv::SetRobotMode_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::SetRobotMode_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::SetRobotMode_Response>()
{
  return rm_interface::srv::builder::Init_SetRobotMode_Response_result();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__BUILDER_HPP_
