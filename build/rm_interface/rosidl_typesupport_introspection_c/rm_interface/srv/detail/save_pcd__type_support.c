// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rm_interface:srv/SavePCD.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rm_interface/srv/detail/save_pcd__rosidl_typesupport_introspection_c.h"
#include "rm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rm_interface/srv/detail/save_pcd__functions.h"
#include "rm_interface/srv/detail/save_pcd__struct.h"


// Include directives for member types
// Member `file_path`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rm_interface__srv__SavePCD_Request__init(message_memory);
}

void rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_fini_function(void * message_memory)
{
  rm_interface__srv__SavePCD_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_member_array[1] = {
  {
    "file_path",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__srv__SavePCD_Request, file_path),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_members = {
  "rm_interface__srv",  // message namespace
  "SavePCD_Request",  // message name
  1,  // number of fields
  sizeof(rm_interface__srv__SavePCD_Request),
  rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_member_array,  // message members
  rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_type_support_handle = {
  0,
  &rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Request)() {
  if (!rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_type_support_handle.typesupport_identifier) {
    rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rm_interface__srv__SavePCD_Request__rosidl_typesupport_introspection_c__SavePCD_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "rm_interface/srv/detail/save_pcd__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "rm_interface/srv/detail/save_pcd__functions.h"
// already included above
// #include "rm_interface/srv/detail/save_pcd__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rm_interface__srv__SavePCD_Response__init(message_memory);
}

void rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_fini_function(void * message_memory)
{
  rm_interface__srv__SavePCD_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_member_array[3] = {
  {
    "memory_gb",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__srv__SavePCD_Response, memory_gb),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "point_num",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__srv__SavePCD_Response, point_num),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__srv__SavePCD_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_members = {
  "rm_interface__srv",  // message namespace
  "SavePCD_Response",  // message name
  3,  // number of fields
  sizeof(rm_interface__srv__SavePCD_Response),
  rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_member_array,  // message members
  rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_type_support_handle = {
  0,
  &rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Response)() {
  if (!rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_type_support_handle.typesupport_identifier) {
    rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rm_interface__srv__SavePCD_Response__rosidl_typesupport_introspection_c__SavePCD_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "rm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rm_interface/srv/detail/save_pcd__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_members = {
  "rm_interface__srv",  // service namespace
  "SavePCD",  // service name
  // these two fields are initialized below on the first access
  NULL,  // request message
  // rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_Request_message_type_support_handle,
  NULL  // response message
  // rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_Response_message_type_support_handle
};

static rosidl_service_type_support_t rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_type_support_handle = {
  0,
  &rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_members,
  get_service_typesupport_handle_function,
};

// Forward declaration of request/response type support functions
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Request)();

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Response)();

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_interface
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD)() {
  if (!rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_type_support_handle.typesupport_identifier) {
    rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, srv, SavePCD_Response)()->data;
  }

  return &rm_interface__srv__detail__save_pcd__rosidl_typesupport_introspection_c__SavePCD_service_type_support_handle;
}
