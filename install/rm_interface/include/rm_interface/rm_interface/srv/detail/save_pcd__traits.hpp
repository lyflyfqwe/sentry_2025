// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_interface:srv/SavePCD.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SAVE_PCD__TRAITS_HPP_
#define RM_INTERFACE__SRV__DETAIL__SAVE_PCD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_interface/srv/detail/save_pcd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const SavePCD_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: file_path
  {
    out << "file_path: ";
    rosidl_generator_traits::value_to_yaml(msg.file_path, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SavePCD_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: file_path
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "file_path: ";
    rosidl_generator_traits::value_to_yaml(msg.file_path, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SavePCD_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace rm_interface

namespace rosidl_generator_traits
{

[[deprecated("use rm_interface::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rm_interface::srv::SavePCD_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::SavePCD_Request & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::SavePCD_Request>()
{
  return "rm_interface::srv::SavePCD_Request";
}

template<>
inline const char * name<rm_interface::srv::SavePCD_Request>()
{
  return "rm_interface/srv/SavePCD_Request";
}

template<>
struct has_fixed_size<rm_interface::srv::SavePCD_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<rm_interface::srv::SavePCD_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<rm_interface::srv::SavePCD_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const SavePCD_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: memory_gb
  {
    out << "memory_gb: ";
    rosidl_generator_traits::value_to_yaml(msg.memory_gb, out);
    out << ", ";
  }

  // member: point_num
  {
    out << "point_num: ";
    rosidl_generator_traits::value_to_yaml(msg.point_num, out);
    out << ", ";
  }

  // member: result
  {
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SavePCD_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: memory_gb
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "memory_gb: ";
    rosidl_generator_traits::value_to_yaml(msg.memory_gb, out);
    out << "\n";
  }

  // member: point_num
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "point_num: ";
    rosidl_generator_traits::value_to_yaml(msg.point_num, out);
    out << "\n";
  }

  // member: result
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SavePCD_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace rm_interface

namespace rosidl_generator_traits
{

[[deprecated("use rm_interface::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rm_interface::srv::SavePCD_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::SavePCD_Response & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::SavePCD_Response>()
{
  return "rm_interface::srv::SavePCD_Response";
}

template<>
inline const char * name<rm_interface::srv::SavePCD_Response>()
{
  return "rm_interface/srv/SavePCD_Response";
}

template<>
struct has_fixed_size<rm_interface::srv::SavePCD_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::srv::SavePCD_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::srv::SavePCD_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rm_interface::srv::SavePCD>()
{
  return "rm_interface::srv::SavePCD";
}

template<>
inline const char * name<rm_interface::srv::SavePCD>()
{
  return "rm_interface/srv/SavePCD";
}

template<>
struct has_fixed_size<rm_interface::srv::SavePCD>
  : std::integral_constant<
    bool,
    has_fixed_size<rm_interface::srv::SavePCD_Request>::value &&
    has_fixed_size<rm_interface::srv::SavePCD_Response>::value
  >
{
};

template<>
struct has_bounded_size<rm_interface::srv::SavePCD>
  : std::integral_constant<
    bool,
    has_bounded_size<rm_interface::srv::SavePCD_Request>::value &&
    has_bounded_size<rm_interface::srv::SavePCD_Response>::value
  >
{
};

template<>
struct is_service<rm_interface::srv::SavePCD>
  : std::true_type
{
};

template<>
struct is_service_request<rm_interface::srv::SavePCD_Request>
  : std::true_type
{
};

template<>
struct is_service_response<rm_interface::srv::SavePCD_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // RM_INTERFACE__SRV__DETAIL__SAVE_PCD__TRAITS_HPP_
