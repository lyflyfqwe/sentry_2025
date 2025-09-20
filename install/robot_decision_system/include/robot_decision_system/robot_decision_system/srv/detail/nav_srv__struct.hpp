// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_HPP_
#define ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_decision_system__srv__NavSrv_Request __attribute__((deprecated))
#else
# define DEPRECATED__robot_decision_system__srv__NavSrv_Request __declspec(deprecated)
#endif

namespace robot_decision_system
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct NavSrv_Request_
{
  using Type = NavSrv_Request_<ContainerAllocator>;

  explicit NavSrv_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
    }
  }

  explicit NavSrv_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
    }
  }

  // field types and members
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _z_type =
    float;
  _z_type z;

  // setters for named parameter idiom
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_decision_system__srv__NavSrv_Request
    std::shared_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_decision_system__srv__NavSrv_Request
    std::shared_ptr<robot_decision_system::srv::NavSrv_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const NavSrv_Request_ & other) const
  {
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->z != other.z) {
      return false;
    }
    return true;
  }
  bool operator!=(const NavSrv_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct NavSrv_Request_

// alias to use template instance with default allocator
using NavSrv_Request =
  robot_decision_system::srv::NavSrv_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_decision_system


#ifndef _WIN32
# define DEPRECATED__robot_decision_system__srv__NavSrv_Response __attribute__((deprecated))
#else
# define DEPRECATED__robot_decision_system__srv__NavSrv_Response __declspec(deprecated)
#endif

namespace robot_decision_system
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct NavSrv_Response_
{
  using Type = NavSrv_Response_<ContainerAllocator>;

  explicit NavSrv_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0l;
    }
  }

  explicit NavSrv_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0l;
    }
  }

  // field types and members
  using _result_type =
    int32_t;
  _result_type result;

  // setters for named parameter idiom
  Type & set__result(
    const int32_t & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_decision_system__srv__NavSrv_Response
    std::shared_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_decision_system__srv__NavSrv_Response
    std::shared_ptr<robot_decision_system::srv::NavSrv_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const NavSrv_Response_ & other) const
  {
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const NavSrv_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct NavSrv_Response_

// alias to use template instance with default allocator
using NavSrv_Response =
  robot_decision_system::srv::NavSrv_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_decision_system

namespace robot_decision_system
{

namespace srv
{

struct NavSrv
{
  using Request = robot_decision_system::srv::NavSrv_Request;
  using Response = robot_decision_system::srv::NavSrv_Response;
};

}  // namespace srv

}  // namespace robot_decision_system

#endif  // ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_HPP_
