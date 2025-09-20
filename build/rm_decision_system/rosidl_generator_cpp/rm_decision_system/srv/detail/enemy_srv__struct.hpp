// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_decision_system:srv/EnemySrv.idl
// generated code does not contain a copyright notice

#ifndef RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_HPP_
#define RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_decision_system__srv__EnemySrv_Request __attribute__((deprecated))
#else
# define DEPRECATED__rm_decision_system__srv__EnemySrv_Request __declspec(deprecated)
#endif

namespace rm_decision_system
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct EnemySrv_Request_
{
  using Type = EnemySrv_Request_<ContainerAllocator>;

  explicit EnemySrv_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x1 = 0.0f;
      this->y1 = 0.0f;
      this->z1 = 0.0f;
      this->x2 = 0.0f;
      this->y2 = 0.0f;
      this->z2 = 0.0f;
      this->enemy_count = 0l;
    }
  }

  explicit EnemySrv_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->x1 = 0.0f;
      this->y1 = 0.0f;
      this->z1 = 0.0f;
      this->x2 = 0.0f;
      this->y2 = 0.0f;
      this->z2 = 0.0f;
      this->enemy_count = 0l;
    }
  }

  // field types and members
  using _x1_type =
    float;
  _x1_type x1;
  using _y1_type =
    float;
  _y1_type y1;
  using _z1_type =
    float;
  _z1_type z1;
  using _x2_type =
    float;
  _x2_type x2;
  using _y2_type =
    float;
  _y2_type y2;
  using _z2_type =
    float;
  _z2_type z2;
  using _enemy_count_type =
    int32_t;
  _enemy_count_type enemy_count;

  // setters for named parameter idiom
  Type & set__x1(
    const float & _arg)
  {
    this->x1 = _arg;
    return *this;
  }
  Type & set__y1(
    const float & _arg)
  {
    this->y1 = _arg;
    return *this;
  }
  Type & set__z1(
    const float & _arg)
  {
    this->z1 = _arg;
    return *this;
  }
  Type & set__x2(
    const float & _arg)
  {
    this->x2 = _arg;
    return *this;
  }
  Type & set__y2(
    const float & _arg)
  {
    this->y2 = _arg;
    return *this;
  }
  Type & set__z2(
    const float & _arg)
  {
    this->z2 = _arg;
    return *this;
  }
  Type & set__enemy_count(
    const int32_t & _arg)
  {
    this->enemy_count = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_decision_system__srv__EnemySrv_Request
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_decision_system__srv__EnemySrv_Request
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EnemySrv_Request_ & other) const
  {
    if (this->x1 != other.x1) {
      return false;
    }
    if (this->y1 != other.y1) {
      return false;
    }
    if (this->z1 != other.z1) {
      return false;
    }
    if (this->x2 != other.x2) {
      return false;
    }
    if (this->y2 != other.y2) {
      return false;
    }
    if (this->z2 != other.z2) {
      return false;
    }
    if (this->enemy_count != other.enemy_count) {
      return false;
    }
    return true;
  }
  bool operator!=(const EnemySrv_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EnemySrv_Request_

// alias to use template instance with default allocator
using EnemySrv_Request =
  rm_decision_system::srv::EnemySrv_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_decision_system


#ifndef _WIN32
# define DEPRECATED__rm_decision_system__srv__EnemySrv_Response __attribute__((deprecated))
#else
# define DEPRECATED__rm_decision_system__srv__EnemySrv_Response __declspec(deprecated)
#endif

namespace rm_decision_system
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct EnemySrv_Response_
{
  using Type = EnemySrv_Response_<ContainerAllocator>;

  explicit EnemySrv_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->result = 0l;
    }
  }

  explicit EnemySrv_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
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
    rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_decision_system__srv__EnemySrv_Response
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_decision_system__srv__EnemySrv_Response
    std::shared_ptr<rm_decision_system::srv::EnemySrv_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const EnemySrv_Response_ & other) const
  {
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const EnemySrv_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct EnemySrv_Response_

// alias to use template instance with default allocator
using EnemySrv_Response =
  rm_decision_system::srv::EnemySrv_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_decision_system

namespace rm_decision_system
{

namespace srv
{

struct EnemySrv
{
  using Request = rm_decision_system::srv::EnemySrv_Request;
  using Response = rm_decision_system::srv::EnemySrv_Response;
};

}  // namespace srv

}  // namespace rm_decision_system

#endif  // RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_HPP_
