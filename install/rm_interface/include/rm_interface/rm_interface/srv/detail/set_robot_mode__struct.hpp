// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_interface:srv/SetRobotMode.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_HPP_
#define RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_interface__srv__SetRobotMode_Request __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__srv__SetRobotMode_Request __declspec(deprecated)
#endif

namespace rm_interface
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetRobotMode_Request_
{
  using Type = SetRobotMode_Request_<ContainerAllocator>;

  explicit SetRobotMode_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->robot_speed_rate = 0.0f;
    }
  }

  explicit SetRobotMode_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->robot_speed_rate = 0.0f;
    }
  }

  // field types and members
  using _robot_speed_rate_type =
    float;
  _robot_speed_rate_type robot_speed_rate;

  // setters for named parameter idiom
  Type & set__robot_speed_rate(
    const float & _arg)
  {
    this->robot_speed_rate = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__srv__SetRobotMode_Request
    std::shared_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__srv__SetRobotMode_Request
    std::shared_ptr<rm_interface::srv::SetRobotMode_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetRobotMode_Request_ & other) const
  {
    if (this->robot_speed_rate != other.robot_speed_rate) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetRobotMode_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetRobotMode_Request_

// alias to use template instance with default allocator
using SetRobotMode_Request =
  rm_interface::srv::SetRobotMode_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_interface


#ifndef _WIN32
# define DEPRECATED__rm_interface__srv__SetRobotMode_Response __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__srv__SetRobotMode_Response __declspec(deprecated)
#endif

namespace rm_interface
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetRobotMode_Response_
{
  using Type = SetRobotMode_Response_<ContainerAllocator>;

  explicit SetRobotMode_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = false;
    }
  }

  explicit SetRobotMode_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = false;
    }
  }

  // field types and members
  using _result_type =
    bool;
  _result_type result;

  // setters for named parameter idiom
  Type & set__result(
    const bool & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__srv__SetRobotMode_Response
    std::shared_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__srv__SetRobotMode_Response
    std::shared_ptr<rm_interface::srv::SetRobotMode_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetRobotMode_Response_ & other) const
  {
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetRobotMode_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetRobotMode_Response_

// alias to use template instance with default allocator
using SetRobotMode_Response =
  rm_interface::srv::SetRobotMode_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_interface

namespace rm_interface
{

namespace srv
{

struct SetRobotMode
{
  using Request = rm_interface::srv::SetRobotMode_Request;
  using Response = rm_interface::srv::SetRobotMode_Response;
};

}  // namespace srv

}  // namespace rm_interface

#endif  // RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_HPP_
