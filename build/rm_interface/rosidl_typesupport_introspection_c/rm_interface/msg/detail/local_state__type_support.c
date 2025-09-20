// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rm_interface/msg/detail/local_state__rosidl_typesupport_introspection_c.h"
#include "rm_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rm_interface/msg/detail/local_state__functions.h"
#include "rm_interface/msg/detail/local_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rm_interface__msg__LocalState__init(message_memory);
}

void rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_fini_function(void * message_memory)
{
  rm_interface__msg__LocalState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_member_array[2] = {
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__msg__LocalState, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "safe_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rm_interface__msg__LocalState, safe_state),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_members = {
  "rm_interface__msg",  // message namespace
  "LocalState",  // message name
  2,  // number of fields
  sizeof(rm_interface__msg__LocalState),
  rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_member_array,  // message members
  rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_init_function,  // function to initialize message memory (memory has to be allocated)
  rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_type_support_handle = {
  0,
  &rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rm_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rm_interface, msg, LocalState)() {
  if (!rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_type_support_handle.typesupport_identifier) {
    rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rm_interface__msg__LocalState__rosidl_typesupport_introspection_c__LocalState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
