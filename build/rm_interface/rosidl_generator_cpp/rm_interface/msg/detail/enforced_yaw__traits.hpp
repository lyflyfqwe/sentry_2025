// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_interface:msg/EnforcedYaw.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__TRAITS_HPP_
#define RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_interface/msg/detail/enforced_yaw__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const EnforcedYaw & msg,
  std::ostream & out)
{
  out << "{";
  // member: flag
  {
    out << "flag: ";
    rosidl_generator_traits::value_to_yaml(msg.flag, out);
    out << ", ";
  }

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
    out << ", ";
  }

  // member: spin
  {
    out << "spin: ";
    rosidl_generator_traits::value_to_yaml(msg.spin, out);
    out << ", ";
  }

  // member: ahead
  {
    out << "ahead: ";
    rosidl_generator_traits::value_to_yaml(msg.ahead, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const EnforcedYaw & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: flag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "flag: ";
    rosidl_generator_traits::value_to_yaml(msg.flag, out);
    out << "\n";
  }

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

  // member: spin
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "spin: ";
    rosidl_generator_traits::value_to_yaml(msg.spin, out);
    out << "\n";
  }

  // member: ahead
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ahead: ";
    rosidl_generator_traits::value_to_yaml(msg.ahead, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const EnforcedYaw & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace rm_interface

namespace rosidl_generator_traits
{

[[deprecated("use rm_interface::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rm_interface::msg::EnforcedYaw & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::msg::EnforcedYaw & msg)
{
  return rm_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::msg::EnforcedYaw>()
{
  return "rm_interface::msg::EnforcedYaw";
}

template<>
inline const char * name<rm_interface::msg::EnforcedYaw>()
{
  return "rm_interface/msg/EnforcedYaw";
}

template<>
struct has_fixed_size<rm_interface::msg::EnforcedYaw>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::msg::EnforcedYaw>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::msg::EnforcedYaw>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__TRAITS_HPP_
