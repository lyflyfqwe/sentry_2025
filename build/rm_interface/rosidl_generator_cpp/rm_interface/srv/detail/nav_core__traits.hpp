// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_interface:srv/NavCore.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__NAV_CORE__TRAITS_HPP_
#define RM_INTERFACE__SRV__DETAIL__NAV_CORE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_interface/srv/detail/nav_core__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavCore_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: z
  {
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const NavCore_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const NavCore_Request & msg, bool use_flow_style = false)
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
  const rm_interface::srv::NavCore_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::NavCore_Request & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::NavCore_Request>()
{
  return "rm_interface::srv::NavCore_Request";
}

template<>
inline const char * name<rm_interface::srv::NavCore_Request>()
{
  return "rm_interface/srv/NavCore_Request";
}

template<>
struct has_fixed_size<rm_interface::srv::NavCore_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::srv::NavCore_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::srv::NavCore_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavCore_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: result
  {
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const NavCore_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
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

inline std::string to_yaml(const NavCore_Response & msg, bool use_flow_style = false)
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
  const rm_interface::srv::NavCore_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::NavCore_Response & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::NavCore_Response>()
{
  return "rm_interface::srv::NavCore_Response";
}

template<>
inline const char * name<rm_interface::srv::NavCore_Response>()
{
  return "rm_interface/srv/NavCore_Response";
}

template<>
struct has_fixed_size<rm_interface::srv::NavCore_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::srv::NavCore_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::srv::NavCore_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rm_interface::srv::NavCore>()
{
  return "rm_interface::srv::NavCore";
}

template<>
inline const char * name<rm_interface::srv::NavCore>()
{
  return "rm_interface/srv/NavCore";
}

template<>
struct has_fixed_size<rm_interface::srv::NavCore>
  : std::integral_constant<
    bool,
    has_fixed_size<rm_interface::srv::NavCore_Request>::value &&
    has_fixed_size<rm_interface::srv::NavCore_Response>::value
  >
{
};

template<>
struct has_bounded_size<rm_interface::srv::NavCore>
  : std::integral_constant<
    bool,
    has_bounded_size<rm_interface::srv::NavCore_Request>::value &&
    has_bounded_size<rm_interface::srv::NavCore_Response>::value
  >
{
};

template<>
struct is_service<rm_interface::srv::NavCore>
  : std::true_type
{
};

template<>
struct is_service_request<rm_interface::srv::NavCore_Request>
  : std::true_type
{
};

template<>
struct is_service_response<rm_interface::srv::NavCore_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // RM_INTERFACE__SRV__DETAIL__NAV_CORE__TRAITS_HPP_
