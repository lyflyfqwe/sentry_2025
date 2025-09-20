// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_HPP_
#define RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_interface__msg__LocalState __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__msg__LocalState __declspec(deprecated)
#endif

namespace rm_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LocalState_
{
  using Type = LocalState_<ContainerAllocator>;

  explicit LocalState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0l;
      this->safe_state = false;
    }
  }

  explicit LocalState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0l;
      this->safe_state = false;
    }
  }

  // field types and members
  using _result_type =
    int32_t;
  _result_type result;
  using _safe_state_type =
    bool;
  _safe_state_type safe_state;

  // setters for named parameter idiom
  Type & set__result(
    const int32_t & _arg)
  {
    this->result = _arg;
    return *this;
  }
  Type & set__safe_state(
    const bool & _arg)
  {
    this->safe_state = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::msg::LocalState_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::msg::LocalState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::msg::LocalState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::msg::LocalState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::LocalState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::LocalState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::msg::LocalState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::msg::LocalState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::msg::LocalState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::msg::LocalState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__msg__LocalState
    std::shared_ptr<rm_interface::msg::LocalState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__msg__LocalState
    std::shared_ptr<rm_interface::msg::LocalState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LocalState_ & other) const
  {
    if (this->result != other.result) {
      return false;
    }
    if (this->safe_state != other.safe_state) {
      return false;
    }
    return true;
  }
  bool operator!=(const LocalState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LocalState_

// alias to use template instance with default allocator
using LocalState =
  rm_interface::msg::LocalState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rm_interface

#endif  // RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_HPP_
