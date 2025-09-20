// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rm_decision_system:srv/EnemySrv.idl
// generated code does not contain a copyright notice

#ifndef RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_H_
#define RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/EnemySrv in the package rm_decision_system.
typedef struct rm_decision_system__srv__EnemySrv_Request
{
  float x1;
  float y1;
  float z1;
  float x2;
  float y2;
  float z2;
  /// 活跃敌人数量
  int32_t enemy_count;
} rm_decision_system__srv__EnemySrv_Request;

// Struct for a sequence of rm_decision_system__srv__EnemySrv_Request.
typedef struct rm_decision_system__srv__EnemySrv_Request__Sequence
{
  rm_decision_system__srv__EnemySrv_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_decision_system__srv__EnemySrv_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/EnemySrv in the package rm_decision_system.
typedef struct rm_decision_system__srv__EnemySrv_Response
{
  /// 1=成功, 2=失败, 3=运行中, 4=未知错误
  int32_t result;
} rm_decision_system__srv__EnemySrv_Response;

// Struct for a sequence of rm_decision_system__srv__EnemySrv_Response.
typedef struct rm_decision_system__srv__EnemySrv_Response__Sequence
{
  rm_decision_system__srv__EnemySrv_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rm_decision_system__srv__EnemySrv_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RM_DECISION_SYSTEM__SRV__DETAIL__ENEMY_SRV__STRUCT_H_
