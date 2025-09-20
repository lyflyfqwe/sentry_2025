// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__TRAITS_HPP_
#define RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_interface/msg/detail/local_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const LocalState & msg,
  std::ostream & out)
{
  out << "{";
  // member: result
  {
    out << "result: ";
    rosidl_generator_traits::value_to_yaml(msg.result, out);
    out << ", ";
  }

  // member: safe_state
  {
    out << "safe_state: ";
    rosidl_generator_traits::value_to_yaml(msg.safe_state, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LocalState & msg,
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

  // member: safe_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "safe_state: ";
    rosidl_generator_traits::value_to_yaml(msg.safe_state, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LocalState & msg, bool use_flow_style = false)
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
  const rm_interface::msg::LocalState & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const rm_interface::msg::LocalState & msg)
{
  return rm_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rm_interface::msg::LocalState>()
{
  return "rm_interface::msg::LocalState";
}

template<>
inline const char * name<rm_interface::msg::LocalState>()
{
  return "rm_interface/msg/LocalState";
}

template<>
struct has_fixed_size<rm_interface::msg::LocalState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_interface::msg::LocalState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_interface::msg::LocalState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__TRAITS_HPP_
