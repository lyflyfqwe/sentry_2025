// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_interface:msg/EnforcedYaw.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_HPP_
#define RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_interface__msg__EnforcedYaw __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__msg__EnforcedYaw __declspec(deprecated)
#endif

namespace rm_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct EnforcedYaw_
{
  using Type = EnforcedYaw_<ContainerAllocator>;

  explicit EnforcedYaw_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->flag = 0l;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->spin = false;
      this->ahead = false;
    }
  }

  explicit EnforcedYaw_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->flag = 0l;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->spin = false;
      this->ahead = false;
    }
  }

  // field types and members
  using _flag_type =
    int32_t;
  _flag_type flag;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _z_type =
    float;
  _z_type z;
  using _spin_type =
    bool;
  _spin_type spin;
  using _ahead_type =
    bool;
  _ahead_type ahead;

  // setters for named parameter idiom
  Type & set__flag(
    const int32_t & _arg)
  {
    this->flag = _arg;
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
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }
  Type & set__spin(
    const bool & _arg)
  {
    this->spin = _arg;
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
    rm_interface::msg::EnforcedYaw_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::msg::EnforcedYaw_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::EnforcedYaw_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::EnforcedYaw_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__msg__EnforcedYaw
    std::shared_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__msg__EnforcedYaw
    std::shared_ptr<rm_interface::msg::EnforcedYaw_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EnforcedYaw_ & other) const
  {
    if (this->flag != other.flag) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->z != other.z) {
      return false;
    }
    if (this->spin != other.spin) {
      return false;
    }
    if (this->ahead != other.ahead) {
      return false;
    }
    return true;
  }
  bool operator!=(const EnforcedYaw_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EnforcedYaw_

// alias to use template instance with default allocator
using EnforcedYaw =
  rm_interface::msg::EnforcedYaw_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_HPP_
