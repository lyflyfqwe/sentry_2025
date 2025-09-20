// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:msg/EnforcedYaw.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__BUILDER_HPP_
#define RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/msg/detail/enforced_yaw__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace msg
{

namespace builder
{

class Init_EnforcedYaw_ahead
{
public:
  explicit Init_EnforcedYaw_ahead(::rm_interface::msg::EnforcedYaw & msg)
  : msg_(msg)
  {}
  ::rm_interface::msg::EnforcedYaw ahead(::rm_interface::msg::EnforcedYaw::_ahead_type arg)
  {
    msg_.ahead = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

class Init_EnforcedYaw_spin
{
public:
  explicit Init_EnforcedYaw_spin(::rm_interface::msg::EnforcedYaw & msg)
  : msg_(msg)
  {}
  Init_EnforcedYaw_ahead spin(::rm_interface::msg::EnforcedYaw::_spin_type arg)
  {
    msg_.spin = std::move(arg);
    return Init_EnforcedYaw_ahead(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

class Init_EnforcedYaw_z
{
public:
  explicit Init_EnforcedYaw_z(::rm_interface::msg::EnforcedYaw & msg)
  : msg_(msg)
  {}
  Init_EnforcedYaw_spin z(::rm_interface::msg::EnforcedYaw::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_EnforcedYaw_spin(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

class Init_EnforcedYaw_y
{
public:
  explicit Init_EnforcedYaw_y(::rm_interface::msg::EnforcedYaw & msg)
  : msg_(msg)
  {}
  Init_EnforcedYaw_z y(::rm_interface::msg::EnforcedYaw::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_EnforcedYaw_z(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

class Init_EnforcedYaw_x
{
public:
  explicit Init_EnforcedYaw_x(::rm_interface::msg::EnforcedYaw & msg)
  : msg_(msg)
  {}
  Init_EnforcedYaw_y x(::rm_interface::msg::EnforcedYaw::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_EnforcedYaw_y(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

class Init_EnforcedYaw_flag
{
public:
  Init_EnforcedYaw_flag()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_EnforcedYaw_x flag(::rm_interface::msg::EnforcedYaw::_flag_type arg)
  {
    msg_.flag = std::move(arg);
    return Init_EnforcedYaw_x(msg_);
  }

private:
  ::rm_interface::msg::EnforcedYaw msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::msg::EnforcedYaw>()
{
  return rm_interface::msg::builder::Init_EnforcedYaw_flag();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__BUILDER_HPP_
