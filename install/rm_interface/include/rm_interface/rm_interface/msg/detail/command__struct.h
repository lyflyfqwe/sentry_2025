// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:msg/Command.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_H_
#define RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/Command in the package rm_interface.
typedef struct rm_interface__msg__Command
{
  /// 有效
  bool available;
  float x;
  float y;
  float r;
  float x_m;
  float y_m;
  /// 0:分离 1:跟随 2:小陀螺
  int16_t robot_mode;
  bool ahead;
} rm_interface__msg__Command;

// Struct for a sequence of rm_interface__msg__Command.
typedef struct rm_interface__msg__Command__Sequence
{
  rm_interface__msg__Command * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__msg__Command__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__MSG__DETAIL__COMMAND__STRUCT_H_
