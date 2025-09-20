// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_interface:srv/SavePCD.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SAVE_PCD__BUILDER_HPP_
#define RM_INTERFACE__SRV__DETAIL__SAVE_PCD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_interface/srv/detail/save_pcd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_SavePCD_Request_file_path
{
public:
  Init_SavePCD_Request_file_path()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rm_interface::srv::SavePCD_Request file_path(::rm_interface::srv::SavePCD_Request::_file_path_type arg)
  {
    msg_.file_path = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::SavePCD_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::SavePCD_Request>()
{
  return rm_interface::srv::builder::Init_SavePCD_Request_file_path();
}

}  // namespace rm_interface


namespace rm_interface
{

namespace srv
{

namespace builder
{

class Init_SavePCD_Response_result
{
public:
  explicit Init_SavePCD_Response_result(::rm_interface::srv::SavePCD_Response & msg)
  : msg_(msg)
  {}
  ::rm_interface::srv::SavePCD_Response result(::rm_interface::srv::SavePCD_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_interface::srv::SavePCD_Response msg_;
};

class Init_SavePCD_Response_point_num
{
public:
  explicit Init_SavePCD_Response_point_num(::rm_interface::srv::SavePCD_Response & msg)
  : msg_(msg)
  {}
  Init_SavePCD_Response_result point_num(::rm_interface::srv::SavePCD_Response::_point_num_type arg)
  {
    msg_.point_num = std::move(arg);
    return Init_SavePCD_Response_result(msg_);
  }

private:
  ::rm_interface::srv::SavePCD_Response msg_;
};

class Init_SavePCD_Response_memory_gb
{
public:
  Init_SavePCD_Response_memory_gb()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SavePCD_Response_point_num memory_gb(::rm_interface::srv::SavePCD_Response::_memory_gb_type arg)
  {
    msg_.memory_gb = std::move(arg);
    return Init_SavePCD_Response_point_num(msg_);
  }

private:
  ::rm_interface::srv::SavePCD_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_interface::srv::SavePCD_Response>()
{
  return rm_interface::srv::builder::Init_SavePCD_Response_memory_gb();
}

}  // namespace rm_interface

#endif  // RM_INTERFACE__SRV__DETAIL__SAVE_PCD__BUILDER_HPP_
