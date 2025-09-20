// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:msg/EnforcedYaw.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_H_
#define RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/EnforcedYaw in the package rm_interface.
typedef struct rm_interface__msg__EnforcedYaw
{
  int32_t flag;
  float x;
  float y;
  float z;
  bool spin;
  bool ahead;
} rm_interface__msg__EnforcedYaw;

// Struct for a sequence of rm_interface__msg__EnforcedYaw.
typedef struct rm_interface__msg__EnforcedYaw__Sequence
{
  rm_interface__msg__EnforcedYaw * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__msg__EnforcedYaw__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__MSG__DETAIL__ENFORCED_YAW__STRUCT_H_
