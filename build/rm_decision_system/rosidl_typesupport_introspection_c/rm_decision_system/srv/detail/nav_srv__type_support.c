// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rm_decision_system:srv/NavSrv.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rm_decision_system/srv/detail/nav_srv__rosidl_typesupport_introspection_c.h"
#include "rm_decision_system/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rm_decision_system/srv/detail/nav_srv__functions.h"
#include "rm_decision_system/srv/detail/nav_srv__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rm_decision_system__srv__NavSrv_Request__init(message_memory);
}

void rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_fini_function(void * message_memory)
{
  rm_decision_system__srv__NavSrv_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_member_array[3] = {
  {
    "x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_decision_system__srv__NavSrv_Request, x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_decision_system__srv__NavSrv_Request, y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "z",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_decision_system__srv__NavSrv_Request, z),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_members = {
  "rm_decision_system__srv",  // message namespace
  "NavSrv_Request",  // message name
  3,  // number of fields
  sizeof(rm_decision_system__srv__NavSrv_Request),
  rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_member_array,  // message members
  rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_type_support_handle = {
  0,
  &rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_decision_system
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Request)() {
  if (!rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_type_support_handle.typesupport_identifier) {
    rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rm_decision_system__srv__NavSrv_Request__rosidl_typesupport_introspection_c__NavSrv_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "rm_decision_system/srv/detail/nav_srv__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rm_decision_system/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "rm_decision_system/srv/detail/nav_srv__functions.h"
// already included above
// #include "rm_decision_system/srv/detail/nav_srv__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rm_decision_system__srv__NavSrv_Response__init(message_memory);
}

void rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_fini_function(void * message_memory)
{
  rm_decision_system__srv__NavSrv_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_member_array[1] = {
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_decision_system__srv__NavSrv_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_members = {
  "rm_decision_system__srv",  // message namespace
  "NavSrv_Response",  // message name
  1,  // number of fields
  sizeof(rm_decision_system__srv__NavSrv_Response),
  rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_member_array,  // message members
  rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_type_support_handle = {
  0,
  &rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_decision_system
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Response)() {
  if (!rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_type_support_handle.typesupport_identifier) {
    rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rm_decision_system__srv__NavSrv_Response__rosidl_typesupport_introspection_c__NavSrv_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "rm_decision_system/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rm_decision_system/srv/detail/nav_srv__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_members = {
  "rm_decision_system__srv",  // service namespace
  "NavSrv",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_Request_message_type_support_handle,
  NULL  // response message
  // rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_Response_message_type_support_handle
};

static rosidl_service_type_support_t rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_type_support_handle = {
  0,
  &rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_decision_system
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv)() {
  if (!rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_type_support_handle.typesupport_identifier) {
    rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_decision_system, srv, NavSrv_Response)()->data;
  }

  return &rm_decision_system__srv__detail__nav_srv__rosidl_typesupport_introspection_c__NavSrv_service_type_support_handle;
}
