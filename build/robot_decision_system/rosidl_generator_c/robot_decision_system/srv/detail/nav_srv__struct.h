// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_H_
#define ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/NavSrv in the package robot_decision_system.
typedef struct robot_decision_system__srv__NavSrv_Request
{
  float x;
  float y;
  float z;
} robot_decision_system__srv__NavSrv_Request;

// Struct for a sequence of robot_decision_system__srv__NavSrv_Request.
typedef struct robot_decision_system__srv__NavSrv_Request__Sequence
{
  robot_decision_system__srv__NavSrv_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_decision_system__srv__NavSrv_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/NavSrv in the package robot_decision_system.
typedef struct robot_decision_system__srv__NavSrv_Response
{
  /// 1=成功, 2=失败, 3=运行中, 4=未知错误
  int32_t result;
} robot_decision_system__srv__NavSrv_Response;

// Struct for a sequence of robot_decision_system__srv__NavSrv_Response.
typedef struct robot_decision_system__srv__NavSrv_Response__Sequence
{
  robot_decision_system__srv__NavSrv_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_decision_system__srv__NavSrv_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_DECISION_SYSTEM__SRV__DETAIL__NAV_SRV__STRUCT_H_
