// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:srv/NavCore.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__NAV_CORE__BUILDER_HPP_
#define RM_INTERFACE__SRV__DETAIL__NAV_CORE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/srv/detail/nav_core__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_NavCore_Request_z
{
public:
  explicit Init_NavCore_Request_z(::rm_interface::srv::NavCore_Request & msg)
  : msg_(msg)
  {}
  ::rm_interface::srv::NavCore_Request z(::rm_interface::srv::NavCore_Request::_z_type arg)
  {
    msg_.z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::NavCore_Request msg_;
};

class Init_NavCore_Request_y
{
public:
  explicit Init_NavCore_Request_y(::rm_interface::srv::NavCore_Request & msg)
  : msg_(msg)
  {}
  Init_NavCore_Request_z y(::rm_interface::srv::NavCore_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_NavCore_Request_z(msg_);
  }

private:
  ::rm_interface::srv::NavCore_Request msg_;
};

class Init_NavCore_Request_x
{
public:
  Init_NavCore_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NavCore_Request_y x(::rm_interface::srv::NavCore_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_NavCore_Request_y(msg_);
  }

private:
  ::rm_interface::srv::NavCore_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::NavCore_Request>()
{
  return rm_interface::srv::builder::Init_NavCore_Request_x();
}

}  // namespace rm_interface


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_NavCore_Response_result
{
public:
  Init_NavCore_Response_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rm_interface::srv::NavCore_Response result(::rm_interface::srv::NavCore_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::NavCore_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::NavCore_Response>()
{
  return rm_interface::srv::builder::Init_NavCore_Response_result();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__SRV__DETAIL__NAV_CORE__BUILDER_HPP_
