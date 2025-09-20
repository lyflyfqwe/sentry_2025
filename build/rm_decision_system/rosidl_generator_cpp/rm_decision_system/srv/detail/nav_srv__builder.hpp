// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#ifndef RM_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_
#define RM_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_decision_system/srv/detail/nav_srv__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_decision_system
{

namespace srv
{

namespace builder
{

class Init_NavSrv_Request_z
{
public:
  explicit Init_NavSrv_Request_z(::rm_decision_system::srv::NavSrv_Request & msg)
  : msg_(msg)
  {}
  ::rm_decision_system::srv::NavSrv_Request z(::rm_decision_system::srv::NavSrv_Request::_z_type arg)
  {
    msg_.z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_decision_system::srv::NavSrv_Request msg_;
};

class Init_NavSrv_Request_y
{
public:
  explicit Init_NavSrv_Request_y(::rm_decision_system::srv::NavSrv_Request & msg)
  : msg_(msg)
  {}
  Init_NavSrv_Request_z y(::rm_decision_system::srv::NavSrv_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_NavSrv_Request_z(msg_);
  }

private:
  ::rm_decision_system::srv::NavSrv_Request msg_;
};

class Init_NavSrv_Request_x
{
public:
  Init_NavSrv_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NavSrv_Request_y x(::rm_decision_system::srv::NavSrv_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_NavSrv_Request_y(msg_);
  }

private:
  ::rm_decision_system::srv::NavSrv_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_decision_system::srv::NavSrv_Request>()
{
  return rm_decision_system::srv::builder::Init_NavSrv_Request_x();
}

}  // namespace rm_decision_system


namespace rm_decision_system
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
  ::rm_decision_system::srv::NavSrv_Response result(::rm_decision_system::srv::NavSrv_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_decision_system::srv::NavSrv_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_decision_system::srv::NavSrv_Response>()
{
  return rm_decision_system::srv::builder::Init_NavSrv_Response_result();
}

}  // namespace rm_decision_system

#endif  // RM_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__BUILDER_HPP_
