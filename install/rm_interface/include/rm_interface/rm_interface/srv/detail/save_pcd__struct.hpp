// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rm_interface:srv/SavePCD.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_HPP_
#define RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rm_interface__srv__SavePCD_Request __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__srv__SavePCD_Request __declspec(deprecated)
#endif

namespace rm_interface
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SavePCD_Request_
{
  using Type = SavePCD_Request_<ContainerAllocator>;

  explicit SavePCD_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->file_path = "";
    }
  }

  explicit SavePCD_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : file_path(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->file_path = "";
    }
  }

  // field types and members
  using _file_path_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _file_path_type file_path;

  // setters for named parameter idiom
  Type & set__file_path(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->file_path = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::srv::SavePCD_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::srv::SavePCD_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SavePCD_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SavePCD_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__srv__SavePCD_Request
    std::shared_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__srv__SavePCD_Request
    std::shared_ptr<rm_interface::srv::SavePCD_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SavePCD_Request_ & other) const
  {
    if (this->file_path != other.file_path) {
      return false;
    }
    return true;
  }
  bool operator!=(const SavePCD_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SavePCD_Request_

// alias to use template instance with default allocator
using SavePCD_Request =
  rm_interface::srv::SavePCD_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_interface


#ifndef _WIN32
# define DEPRECATED__rm_interface__srv__SavePCD_Response __attribute__((deprecated))
#else
# define DEPRECATED__rm_interface__srv__SavePCD_Response __declspec(deprecated)
#endif

namespace rm_interface
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SavePCD_Response_
{
  using Type = SavePCD_Response_<ContainerAllocator>;

  explicit SavePCD_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->memory_gb = 0.0f;
      this->point_num = 0l;
      this->result = 0l;
    }
  }

  explicit SavePCD_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->memory_gb = 0.0f;
      this->point_num = 0l;
      this->result = 0l;
    }
  }

  // field types and members
  using _memory_gb_type =
    float;
  _memory_gb_type memory_gb;
  using _point_num_type =
    int32_t;
  _point_num_type point_num;
  using _result_type =
    int32_t;
  _result_type result;

  // setters for named parameter idiom
  Type & set__memory_gb(
    const float & _arg)
  {
    this->memory_gb = _arg;
    return *this;
  }
  Type & set__point_num(
    const int32_t & _arg)
  {
    this->point_num = _arg;
    return *this;
  }
  Type & set__result(
    const int32_t & _arg)
  {
    this->result = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rm_interface::srv::SavePCD_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const rm_interface::srv::SavePCD_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SavePCD_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rm_interface::srv::SavePCD_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rm_interface__srv__SavePCD_Response
    std::shared_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rm_interface__srv__SavePCD_Response
    std::shared_ptr<rm_interface::srv::SavePCD_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SavePCD_Response_ & other) const
  {
    if (this->memory_gb != other.memory_gb) {
      return false;
    }
    if (this->point_num != other.point_num) {
      return false;
    }
    if (this->result != other.result) {
      return false;
    }
    return true;
  }
  bool operator!=(const SavePCD_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SavePCD_Response_

// alias to use template instance with default allocator
using SavePCD_Response =
  rm_interface::srv::SavePCD_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace rm_interface

namespace rm_interface
{

namespace srv
{

struct SavePCD
{
  using Request = rm_interface::srv::SavePCD_Request;
  using Response = rm_interface::srv::SavePCD_Response;
};

}  // namespace srv

}  // namespace rm_interface

#endif  // RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_HPP_
