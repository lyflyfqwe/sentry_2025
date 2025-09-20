// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:srv/SetRobotMode.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_H_
#define RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetRobotMode in the package rm_interface.
typedef struct rm_interface__srv__SetRobotMode_Request
{
  /// 速度档位 (0.5~2.5)
  float robot_speed_rate;
} rm_interface__srv__SetRobotMode_Request;

// Struct for a sequence of rm_interface__srv__SetRobotMode_Request.
typedef struct rm_interface__srv__SetRobotMode_Request__Sequence
{
  rm_interface__srv__SetRobotMode_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__SetRobotMode_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/SetRobotMode in the package rm_interface.
typedef struct rm_interface__srv__SetRobotMode_Response
{
  bool result;
} rm_interface__srv__SetRobotMode_Response;

// Struct for a sequence of rm_interface__srv__SetRobotMode_Response.
typedef struct rm_interface__srv__SetRobotMode_Response__Sequence
{
  rm_interface__srv__SetRobotMode_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__SetRobotMode_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__SRV__DETAIL__SET_ROBOT_MODE__STRUCT_H_
