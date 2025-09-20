// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_interface:msg/Command.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_HPP_
#define RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_interface__msg__Command __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__msg__Command __declspec(deprecated)
#endif

namespace rm_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Command_
{
  using Type = Command_<ContainerAllocator>;

  explicit Command_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->available = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->r = 0.0f;
      this->x_m = 0.0f;
      this->y_m = 0.0f;
      this->robot_mode = 0;
      this->ahead = false;
    }
  }

  explicit Command_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->available = false;
      this->x = 0.0f;
      this->y = 0.0f;
      this->r = 0.0f;
      this->x_m = 0.0f;
      this->y_m = 0.0f;
      this->robot_mode = 0;
      this->ahead = false;
    }
  }

  // field types and members
  using _available_type =
    bool;
  _available_type available;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _r_type =
    float;
  _r_type r;
  using _x_m_type =
    float;
  _x_m_type x_m;
  using _y_m_type =
    float;
  _y_m_type y_m;
  using _robot_mode_type =
    int16_t;
  _robot_mode_type robot_mode;
  using _ahead_type =
    bool;
  _ahead_type ahead;

  // setters for named parameter idiom
  Type & set__available(
    const bool & _arg)
  {
    this->available = _arg;
    return *this;
  }
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
  Type & set__r(
    const float & _arg)
  {
    this->r = _arg;
    return *this;
  }
  Type & set__x_m(
    const float & _arg)
  {
    this->x_m = _arg;
    return *this;
  }
  Type & set__y_m(
    const float & _arg)
  {
    this->y_m = _arg;
    return *this;
  }
  Type & set__robot_mode(
    const int16_t & _arg)
  {
    this->robot_mode = _arg;
    return *this;
  }
  Type & set__ahead(
    const bool & _arg)
  {
    this->ahead = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::msg::Command_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::msg::Command_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::msg::Command_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::msg::Command_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::Command_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::Command_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::Command_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::Command_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::msg::Command_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::msg::Command_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__msg__Command
    std::shared_ptr<rm_interface::msg::Command_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__msg__Command
    std::shared_ptr<rm_interface::msg::Command_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Command_ & other) const
  {
    if (this->available != other.available) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->r != other.r) {
      return false;
    }
    if (this->x_m != other.x_m) {
      return false;
    }
    if (this->y_m != other.y_m) {
      return false;
    }
    if (this->robot_mode != other.robot_mode) {
      return false;
    }
    if (this->ahead != other.ahead) {
      return false;
    }
    return true;
  }
  bool operator!=(const Command_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Command_

// alias to use template instance with default allocator
using Command =
  rm_interface::msg::Command_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_HPP_
