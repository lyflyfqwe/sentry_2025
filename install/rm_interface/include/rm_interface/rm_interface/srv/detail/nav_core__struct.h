// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_interface:srv/NavCore.idl
// generated code does not contain a copyright notice

#ifndef RM_INTERFACE__SRV__DETAIL__NAV_CORE__STRUCT_H_
#define RM_INTERFACE__SRV__DETAIL__NAV_CORE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/NavCore in the package rm_interface.
typedef struct rm_interface__srv__NavCore_Request
{
  float x;
  float y;
  float z;
} rm_interface__srv__NavCore_Request;

// Struct for a sequence of rm_interface__srv__NavCore_Request.
typedef struct rm_interface__srv__NavCore_Request__Sequence
{
  rm_interface__srv__NavCore_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__NavCore_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/NavCore in the package rm_interface.
typedef struct rm_interface__srv__NavCore_Response
{
  int32_t result;
} rm_interface__srv__NavCore_Response;

// Struct for a sequence of rm_interface__srv__NavCore_Response.
typedef struct rm_interface__srv__NavCore_Response__Sequence
{
  rm_interface__srv__NavCore_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_interface__srv__NavCore_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_INTERFACE__SRV__DETAIL__NAV_CORE__STRUCT_H_
