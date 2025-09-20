// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from rm_interface:msg/Command.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "rm_interface/msg/detail/command__struct.h"
#include "rm_interface/msg/detail/command__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool rm_interface__msg__command__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[34];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("rm_interface.msg._command.Command", full_classname_dest, 33) == 0);
  }
  rm_interface__msg__Command * ros_message = _ros_message;
  {  // available
    PyObject * field = PyObject_GetAttrString(_pymsg, "available");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->available = (Py_True == field);
    Py_DECREF(field);
  }
  {  // x
    PyObject * field = PyObject_GetAttrString(_pymsg, "x");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->x = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // y
    PyObject * field = PyObject_GetAttrString(_pymsg, "y");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->y = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // r
    PyObject * field = PyObject_GetAttrString(_pymsg, "r");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->r = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // x_m
    PyObject * field = PyObject_GetAttrString(_pymsg, "x_m");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->x_m = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // y_m
    PyObject * field = PyObject_GetAttrString(_pymsg, "y_m");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->y_m = (float)PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // robot_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "robot_mode");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->robot_mode = (int16_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // ahead
    PyObject * field = PyObject_GetAttrString(_pymsg, "ahead");
    if (!field) {
      return false;
    }
    assert(PyBool_Check(field));
    ros_message->ahead = (Py_True == field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * rm_interface__msg__command__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of Command */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("rm_interface.msg._command");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "Command");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  rm_interface__msg__Command * ros_message = (rm_interface__msg__Command *)raw_ros_message;
  {  // available
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->available ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "available", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->y);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // r
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->r);
    {
      int rc = PyObject_SetAttrString(_pymessage, "r", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x_m
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->x_m);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x_m", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // y_m
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->y_m);
    {
      int rc = PyObject_SetAttrString(_pymessage, "y_m", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // robot_mode
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->robot_mode);
    {
      int rc = PyObject_SetAttrString(_pymessage, "robot_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ahead
    PyObject * field = NULL;
    field = PyBool_FromLong(ros_message->ahead ? 1 : 0);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ahead", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
