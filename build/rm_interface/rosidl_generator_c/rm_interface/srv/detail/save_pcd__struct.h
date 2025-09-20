// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:srv/SavePCD.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_H_
#define RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'file_path'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SavePCD in the package rm_interface.
typedef struct rm_interface__srv__SavePCD_Request
{
  /// 点云pcd保存路径
  rosidl_runtime_c__String file_path;
} rm_interface__srv__SavePCD_Request;

// Struct for a sequence of rm_interface__srv__SavePCD_Request.
typedef struct rm_interface__srv__SavePCD_Request__Sequence
{
  rm_interface__srv__SavePCD_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__SavePCD_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/SavePCD in the package rm_interface.
typedef struct rm_interface__srv__SavePCD_Response
{
  /// 点云文件大小
  float memory_gb;
  int32_t point_num;
  /// 点云保存路径
  int32_t result;
} rm_interface__srv__SavePCD_Response;

// Struct for a sequence of rm_interface__srv__SavePCD_Response.
typedef struct rm_interface__srv__SavePCD_Response__Sequence
{
  rm_interface__srv__SavePCD_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__SavePCD_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__SRV__DETAIL__SAVE_PCD__STRUCT_H_
