// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_interface:srv/SetRobotMode.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__TRAITS_HPP_
#define RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_interface/srv/detail/set_robot_mode__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetRobotMode_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: robot_speed_rate
  {
    out << "robot_speed_rate: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_speed_rate, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetRobotMode_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: robot_speed_rate
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "robot_speed_rate: ";
    rosidl_generator_traits::value_to_yaml(msg.robot_speed_rate, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetRobotMode_Request & msg, bool use_flow_style = false)
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
  const rm_interface::srv::SetRobotMode_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::SetRobotMode_Request & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::SetRobotMode_Request>()
{
  return "rm_interface::srv::SetRobotMode_Request";
}

template<>
inline const char * name<rm_interface::srv::SetRobotMode_Request>()
{
  return "rm_interface/srv/SetRobotMode_Request";
}

template<>
struct has_fixed_size<rm_interface::srv::SetRobotMode_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::srv::SetRobotMode_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::srv::SetRobotMode_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rm_interface
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetRobotMode_Response & msg,
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
  const SetRobotMode_Response & msg,
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

inline std::string to_yaml(const SetRobotMode_Response & msg, bool use_flow_style = false)
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
  const rm_interface::srv::SetRobotMode_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::srv::SetRobotMode_Response & msg)
{
  return rm_interface::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::srv::SetRobotMode_Response>()
{
  return "rm_interface::srv::SetRobotMode_Response";
}

template<>
inline const char * name<rm_interface::srv::SetRobotMode_Response>()
{
  return "rm_interface/srv/SetRobotMode_Response";
}

template<>
struct has_fixed_size<rm_interface::srv::SetRobotMode_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::srv::SetRobotMode_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::srv::SetRobotMode_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rm_interface::srv::SetRobotMode>()
{
  return "rm_interface::srv::SetRobotMode";
}

template<>
inline const char * name<rm_interface::srv::SetRobotMode>()
{
  return "rm_interface/srv/SetRobotMode";
}

template<>
struct has_fixed_size<rm_interface::srv::SetRobotMode>
  : std::integral_constant<
    bool,
    has_fixed_size<rm_interface::srv::SetRobotMode_Request>::value &&
    has_fixed_size<rm_interface::srv::SetRobotMode_Response>::value
  >
{
};

template<>
struct has_bounded_size<rm_interface::srv::SetRobotMode>
  : std::integral_constant<
    bool,
    has_bounded_size<rm_interface::srv::SetRobotMode_Request>::value &&
    has_bounded_size<rm_interface::srv::SetRobotMode_Response>::value
  >
{
};

template<>
struct is_service<rm_interface::srv::SetRobotMode>
  : std::true_type
{
};

template<>
struct is_service_request<rm_interface::srv::SetRobotMode_Request>
  : std::true_type
{
};

template<>
struct is_service_response<rm_interface::srv::SetRobotMode_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__TRAITS_HPP_
