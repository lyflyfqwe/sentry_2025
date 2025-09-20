// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_
#define ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_decision_system/srv/detail/nav_srv__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_decision_system
{

namespace srv
{

namespace builder
{

class Init_NavSrv_Request_z
{
public:
  explicit Init_NavSrv_Request_z(::robot_decision_system::srv::NavSrv_Request & msg)
  : msg_(msg)
  {}
  ::robot_decision_system::srv::NavSrv_Request z(::robot_decision_system::srv::NavSrv_Request::_z_type arg)
  {
    msg_.z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_decision_system::srv::NavSrv_Request msg_;
};

class Init_NavSrv_Request_y
{
public:
  explicit Init_NavSrv_Request_y(::robot_decision_system::srv::NavSrv_Request & msg)
  : msg_(msg)
  {}
  Init_NavSrv_Request_z y(::robot_decision_system::srv::NavSrv_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_NavSrv_Request_z(msg_);
  }

private:
  ::robot_decision_system::srv::NavSrv_Request msg_;
};

class Init_NavSrv_Request_x
{
public:
  Init_NavSrv_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NavSrv_Request_y x(::robot_decision_system::srv::NavSrv_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_NavSrv_Request_y(msg_);
  }

private:
  ::robot_decision_system::srv::NavSrv_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_decision_system::srv::NavSrv_Request>()
{
  return robot_decision_system::srv::builder::Init_NavSrv_Request_x();
}

}  // namespace robot_decision_system


namespace robot_decision_system
{

namespace srv
{

namespace builder
{

class Init_NavSrv_Response_result
{
public:
  Init_NavSrv_Response_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_decision_system::srv::NavSrv_Response result(::robot_decision_system::srv::NavSrv_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_decision_system::srv::NavSrv_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_decision_system::srv::NavSrv_Response>()
{
  return robot_decision_system::srv::builder::Init_NavSrv_Response_result();
}

}  // namespace robot_decision_system

#endif  // ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_
