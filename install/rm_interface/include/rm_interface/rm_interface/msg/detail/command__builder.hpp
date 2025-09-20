// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:msg/Command.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__COMMAND__BUILDER_HPP_
#define RM_INTERFACE__MSG__DETAIL__COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/msg/detail/command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace msg
{

namespace builder
{

class Init_Command_ahead
{
public:
  explicit Init_Command_ahead(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  ::rm_interface::msg::Command ahead(::rm_interface::msg::Command::_ahead_type arg)
  {
    msg_.ahead = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_robot_mode
{
public:
  explicit Init_Command_robot_mode(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_ahead robot_mode(::rm_interface::msg::Command::_robot_mode_type arg)
  {
    msg_.robot_mode = std::move(arg);
    return Init_Command_ahead(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_y_m
{
public:
  explicit Init_Command_y_m(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_robot_mode y_m(::rm_interface::msg::Command::_y_m_type arg)
  {
    msg_.y_m = std::move(arg);
    return Init_Command_robot_mode(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_x_m
{
public:
  explicit Init_Command_x_m(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_y_m x_m(::rm_interface::msg::Command::_x_m_type arg)
  {
    msg_.x_m = std::move(arg);
    return Init_Command_y_m(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_r
{
public:
  explicit Init_Command_r(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_x_m r(::rm_interface::msg::Command::_r_type arg)
  {
    msg_.r = std::move(arg);
    return Init_Command_x_m(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_y
{
public:
  explicit Init_Command_y(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_r y(::rm_interface::msg::Command::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_Command_r(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_x
{
public:
  explicit Init_Command_x(::rm_interface::msg::Command & msg)
  : msg_(msg)
  {}
  Init_Command_y x(::rm_interface::msg::Command::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Command_y(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

class Init_Command_available
{
public:
  Init_Command_available()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Command_x available(::rm_interface::msg::Command::_available_type arg)
  {
    msg_.available = std::move(arg);
    return Init_Command_x(msg_);
  }

private:
  ::rm_interface::msg::Command msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::msg::Command>()
{
  return rm_interface::msg::builder::Init_Command_available();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__COMMAND__BUILDER_HPP_
