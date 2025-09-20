// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__TRAITS_HPP_
#define ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_decision_system/srv/detail/nav_srv__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace robot_decision_system
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavSrv_Request & msg,
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
  const NavSrv_Request & msg,
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

inline std::string to_yaml(const NavSrv_Request & msg, bool use_flow_style = false)
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

}  // namespace robot_decision_system

namespace rosidl_generator_traits
{

[[deprecated("use robot_decision_system::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_decision_system::srv::NavSrv_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_decision_system::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_decision_system::srv::to_yaml() instead")]]
inline std::string to_yaml(const robot_decision_system::srv::NavSrv_Request & msg)
{
  return robot_decision_system::srv::to_yaml(msg);
}

template<>
inline const char * data_type<robot_decision_system::srv::NavSrv_Request>()
{
  return "robot_decision_system::srv::NavSrv_Request";
}

template<>
inline const char * name<robot_decision_system::srv::NavSrv_Request>()
{
  return "robot_decision_system/srv/NavSrv_Request";
}

template<>
struct has_fixed_size<robot_decision_system::srv::NavSrv_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<robot_decision_system::srv::NavSrv_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<robot_decision_system::srv::NavSrv_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace robot_decision_system
{

namespace srv
{

inline void to_flow_style_yaml(
  const NavSrv_Response & msg,
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
  const NavSrv_Response & msg,
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

inline std::string to_yaml(const NavSrv_Response & msg, bool use_flow_style = false)
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

}  // namespace robot_decision_system

namespace rosidl_generator_traits
{

[[deprecated("use robot_decision_system::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_decision_system::srv::NavSrv_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_decision_system::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_decision_system::srv::to_yaml() instead")]]
inline std::string to_yaml(const robot_decision_system::srv::NavSrv_Response & msg)
{
  return robot_decision_system::srv::to_yaml(msg);
}

template<>
inline const char * data_type<robot_decision_system::srv::NavSrv_Response>()
{
  return "robot_decision_system::srv::NavSrv_Response";
}

template<>
inline const char * name<robot_decision_system::srv::NavSrv_Response>()
{
  return "robot_decision_system/srv/NavSrv_Response";
}

template<>
struct has_fixed_size<robot_decision_system::srv::NavSrv_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<robot_decision_system::srv::NavSrv_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<robot_decision_system::srv::NavSrv_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<robot_decision_system::srv::NavSrv>()
{
  return "robot_decision_system::srv::NavSrv";
}

template<>
inline const char * name<robot_decision_system::srv::NavSrv>()
{
  return "robot_decision_system/srv/NavSrv";
}

template<>
struct has_fixed_size<robot_decision_system::srv::NavSrv>
  : std::integral_constant<
    bool,
    has_fixed_size<robot_decision_system::srv::NavSrv_Request>::value &&
    has_fixed_size<robot_decision_system::srv::NavSrv_Response>::value
  >
{
};

template<>
struct has_bounded_size<robot_decision_system::srv::NavSrv>
  : std::integral_constant<
    bool,
    has_bounded_size<robot_decision_system::srv::NavSrv_Request>::value &&
    has_bounded_size<robot_decision_system::srv::NavSrv_Response>::value
  >
{
};

template<>
struct is_service<robot_decision_system::srv::NavSrv>
  : std::true_type
{
};

template<>
struct is_service_request<robot_decision_system::srv::NavSrv_Request>
  : std::true_type
{
};

template<>
struct is_service_response<robot_decision_system::srv::NavSrv_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__TRAITS_HPP_
