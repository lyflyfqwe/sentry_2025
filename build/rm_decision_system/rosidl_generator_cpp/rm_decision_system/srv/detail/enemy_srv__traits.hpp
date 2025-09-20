// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rm_decision_system:srv/EnemySrv.idl
// generated code does not contain a copyright notice

#ifndef RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__TRAITS_HPP_
#define RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rm_decision_system/srv/detail/enemy_srv__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rm_decision_system
{

namespace srv
{

inline void to_flow_style_yaml(
  const EnemySrv_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: x1
  {
    out << "x1: ";
    rosidl_generator_traits::value_to_yaml(msg.x1, out);
    out << ", ";
  }

  // member: y1
  {
    out << "y1: ";
    rosidl_generator_traits::value_to_yaml(msg.y1, out);
    out << ", ";
  }

  // member: z1
  {
    out << "z1: ";
    rosidl_generator_traits::value_to_yaml(msg.z1, out);
    out << ", ";
  }

  // member: x2
  {
    out << "x2: ";
    rosidl_generator_traits::value_to_yaml(msg.x2, out);
    out << ", ";
  }

  // member: y2
  {
    out << "y2: ";
    rosidl_generator_traits::value_to_yaml(msg.y2, out);
    out << ", ";
  }

  // member: z2
  {
    out << "z2: ";
    rosidl_generator_traits::value_to_yaml(msg.z2, out);
    out << ", ";
  }

  // member: enemy_count
  {
    out << "enemy_count: ";
    rosidl_generator_traits::value_to_yaml(msg.enemy_count, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const EnemySrv_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: x1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x1: ";
    rosidl_generator_traits::value_to_yaml(msg.x1, out);
    out << "\n";
  }

  // member: y1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y1: ";
    rosidl_generator_traits::value_to_yaml(msg.y1, out);
    out << "\n";
  }

  // member: z1
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z1: ";
    rosidl_generator_traits::value_to_yaml(msg.z1, out);
    out << "\n";
  }

  // member: x2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x2: ";
    rosidl_generator_traits::value_to_yaml(msg.x2, out);
    out << "\n";
  }

  // member: y2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y2: ";
    rosidl_generator_traits::value_to_yaml(msg.y2, out);
    out << "\n";
  }

  // member: z2
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z2: ";
    rosidl_generator_traits::value_to_yaml(msg.z2, out);
    out << "\n";
  }

  // member: enemy_count
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "enemy_count: ";
    rosidl_generator_traits::value_to_yaml(msg.enemy_count, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const EnemySrv_Request & msg, bool use_flow_style = false)
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

}  // namespace rm_decision_system

namespace rosidl_generator_traits
{

[[deprecated("use rm_decision_system::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rm_decision_system::srv::EnemySrv_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_decision_system::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_decision_system::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_decision_system::srv::EnemySrv_Request & msg)
{
  return rm_decision_system::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_decision_system::srv::EnemySrv_Request>()
{
  return "rm_decision_system::srv::EnemySrv_Request";
}

template<>
inline const char * name<rm_decision_system::srv::EnemySrv_Request>()
{
  return "rm_decision_system/srv/EnemySrv_Request";
}

template<>
struct has_fixed_size<rm_decision_system::srv::EnemySrv_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_decision_system::srv::EnemySrv_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_decision_system::srv::EnemySrv_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rm_decision_system
{

namespace srv
{

inline void to_flow_style_yaml(
  const EnemySrv_Response & msg,
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
  const EnemySrv_Response & msg,
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

inline std::string to_yaml(const EnemySrv_Response & msg, bool use_flow_style = false)
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

}  // namespace rm_decision_system

namespace rosidl_generator_traits
{

[[deprecated("use rm_decision_system::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rm_decision_system::srv::EnemySrv_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  rm_decision_system::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rm_decision_system::srv::to_yaml() instead")]]
inline std::string to_yaml(const rm_decision_system::srv::EnemySrv_Response & msg)
{
  return rm_decision_system::srv::to_yaml(msg);
}

template<>
inline const char * data_type<rm_decision_system::srv::EnemySrv_Response>()
{
  return "rm_decision_system::srv::EnemySrv_Response";
}

template<>
inline const char * name<rm_decision_system::srv::EnemySrv_Response>()
{
  return "rm_decision_system/srv/EnemySrv_Response";
}

template<>
struct has_fixed_size<rm_decision_system::srv::EnemySrv_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rm_decision_system::srv::EnemySrv_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rm_decision_system::srv::EnemySrv_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<rm_decision_system::srv::EnemySrv>()
{
  return "rm_decision_system::srv::EnemySrv";
}

template<>
inline const char * name<rm_decision_system::srv::EnemySrv>()
{
  return "rm_decision_system/srv/EnemySrv";
}

template<>
struct has_fixed_size<rm_decision_system::srv::EnemySrv>
  : std::integral_constant<
    bool,
    has_fixed_size<rm_decision_system::srv::EnemySrv_Request>::value &&
    has_fixed_size<rm_decision_system::srv::EnemySrv_Response>::value
  >
{
};

template<>
struct has_bounded_size<rm_decision_system::srv::EnemySrv>
  : std::integral_constant<
    bool,
    has_bounded_size<rm_decision_system::srv::EnemySrv_Request>::value &&
    has_bounded_size<rm_decision_system::srv::EnemySrv_Response>::value
  >
{
};

template<>
struct is_service<rm_decision_system::srv::EnemySrv>
  : std::true_type
{
};

template<>
struct is_service_request<rm_decision_system::srv::EnemySrv_Request>
  : std::true_type
{
};

template<>
struct is_service_response<rm_decision_system::srv::EnemySrv_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__TRAITS_HPP_
