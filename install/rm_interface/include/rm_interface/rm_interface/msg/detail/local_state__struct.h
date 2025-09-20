// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_H_
#define RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/LocalState in the package rm_interface.
typedef struct rm_interface__msg__LocalState
{
  /// lcoal_path_planning state
  int32_t result;
  /// false: follow mode +
  bool safe_state;
} rm_interface__msg__LocalState;

// Struct for a sequence of rm_interface__msg__LocalState.
typedef struct rm_interface__msg__LocalState__Sequence
{
  rm_interface__msg__LocalState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__msg__LocalState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__MSG__DETAIL__LOCAL_STATE__STRUCT_H_
