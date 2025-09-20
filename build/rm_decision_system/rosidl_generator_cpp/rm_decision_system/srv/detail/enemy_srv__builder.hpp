// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rm_decision_system:srv/EnemySrv.idl
// generated code does not contain a copyright notice

#ifndef RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__BUILDER_HPP_
#define RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rm_decision_system/srv/detail/enemy_srv__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rm_decision_system
{

namespace srv
{

namespace builder
{

class Init_EnemySrv_Request_enemy_count
{
public:
  explicit Init_EnemySrv_Request_enemy_count(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  ::rm_decision_system::srv::EnemySrv_Request enemy_count(::rm_decision_system::srv::EnemySrv_Request::_enemy_count_type arg)
  {
    msg_.enemy_count = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_z2
{
public:
  explicit Init_EnemySrv_Request_z2(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  Init_EnemySrv_Request_enemy_count z2(::rm_decision_system::srv::EnemySrv_Request::_z2_type arg)
  {
    msg_.z2 = std::move(arg);
    return Init_EnemySrv_Request_enemy_count(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_y2
{
public:
  explicit Init_EnemySrv_Request_y2(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  Init_EnemySrv_Request_z2 y2(::rm_decision_system::srv::EnemySrv_Request::_y2_type arg)
  {
    msg_.y2 = std::move(arg);
    return Init_EnemySrv_Request_z2(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_x2
{
public:
  explicit Init_EnemySrv_Request_x2(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  Init_EnemySrv_Request_y2 x2(::rm_decision_system::srv::EnemySrv_Request::_x2_type arg)
  {
    msg_.x2 = std::move(arg);
    return Init_EnemySrv_Request_y2(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_z1
{
public:
  explicit Init_EnemySrv_Request_z1(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  Init_EnemySrv_Request_x2 z1(::rm_decision_system::srv::EnemySrv_Request::_z1_type arg)
  {
    msg_.z1 = std::move(arg);
    return Init_EnemySrv_Request_x2(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_y1
{
public:
  explicit Init_EnemySrv_Request_y1(::rm_decision_system::srv::EnemySrv_Request & msg)
  : msg_(msg)
  {}
  Init_EnemySrv_Request_z1 y1(::rm_decision_system::srv::EnemySrv_Request::_y1_type arg)
  {
    msg_.y1 = std::move(arg);
    return Init_EnemySrv_Request_z1(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

class Init_EnemySrv_Request_x1
{
public:
  Init_EnemySrv_Request_x1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_EnemySrv_Request_y1 x1(::rm_decision_system::srv::EnemySrv_Request::_x1_type arg)
  {
    msg_.x1 = std::move(arg);
    return Init_EnemySrv_Request_y1(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_decision_system::srv::EnemySrv_Request>()
{
  return rm_decision_system::srv::builder::Init_EnemySrv_Request_x1();
}

}  // namespace rm_decision_system


namespace rm_decision_system
{

namespace srv
{

namespace builder
{

class Init_EnemySrv_Response_result
{
public:
  Init_EnemySrv_Response_result()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::rm_decision_system::srv::EnemySrv_Response result(::rm_decision_system::srv::EnemySrv_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rm_decision_system::srv::EnemySrv_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::rm_decision_system::srv::EnemySrv_Response>()
{
  return rm_decision_system::srv::builder::Init_EnemySrv_Response_result();
}

}  // namespace rm_decision_system

#endif  // RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__BUILDER_HPP_
