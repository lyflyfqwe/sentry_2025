// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__BUILDER_HPP_
#define RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/msg/detail/local_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace msg
{

namespace builder
{

class Init_LocalState_safe_state
{
public:
  explicit Init_LocalState_safe_state(::rm_interface::msg::LocalState & msg)
  : msg_(msg)
  {}
  ::rm_interface::msg::LocalState safe_state(::rm_interface::msg::LocalState::_safe_state_type arg)
  {
    msg_.safe_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::msg::LocalState msg_;
};

class Init_LocalState_result
{
public:
  Init_LocalState_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LocalState_safe_state result(::rm_interface::msg::LocalState::_result_type arg)
  {
    msg_.result = std::move(arg);
    return Init_LocalState_safe_state(msg_);
  }

private:
  ::rm_interface::msg::LocalState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::msg::LocalState>()
{
  return rm_interface::msg::builder::Init_LocalState_result();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__BUILDER_HPP_
