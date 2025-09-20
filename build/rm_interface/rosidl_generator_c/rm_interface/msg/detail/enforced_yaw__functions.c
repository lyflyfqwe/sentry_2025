// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rm_interface:msg/EnforcedYaw.idl
// generated code does not contain a copyright notice
#include "rm_interface/msg/detail/enforced_yaw__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rm_interface__msg__EnforcedYaw__init(rm_interface__msg__EnforcedYaw * msg)
{
  if (!msg) {
    return false;
  }
  // flag
  // x
  // y
  // z
  // spin
  // ahead
  return true;
}

void
rm_interface__msg__EnforcedYaw__fini(rm_interface__msg__EnforcedYaw * msg)
{
  if (!msg) {
    return;
  }
  // flag
  // x
  // y
  // z
  // spin
  // ahead
}

bool
rm_interface__msg__EnforcedYaw__are_equal(const rm_interface__msg__EnforcedYaw * lhs, const rm_interface__msg__EnforcedYaw * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // flag
  if (lhs->flag != rhs->flag) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // z
  if (lhs->z != rhs->z) {
    return false;
  }
  // spin
  if (lhs->spin != rhs->spin) {
    return false;
  }
  // ahead
  if (lhs->ahead != rhs->ahead) {
    return false;
  }
  return true;
}

bool
rm_interface__msg__EnforcedYaw__copy(
  const rm_interface__msg__EnforcedYaw * input,
  rm_interface__msg__EnforcedYaw * output)
{
  if (!input || !output) {
    return false;
  }
  // flag
  output->flag = input->flag;
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // z
  output->z = input->z;
  // spin
  output->spin = input->spin;
  // ahead
  output->ahead = input->ahead;
  return true;
}

rm_interface__msg__EnforcedYaw *
rm_interface__msg__EnforcedYaw__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__EnforcedYaw * msg = (rm_interface__msg__EnforcedYaw *)allocator.allocate(sizeof(rm_interface__msg__EnforcedYaw), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rm_interface__msg__EnforcedYaw));
  bool success = rm_interface__msg__EnforcedYaw__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rm_interface__msg__EnforcedYaw__destroy(rm_interface__msg__EnforcedYaw * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rm_interface__msg__EnforcedYaw__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rm_interface__msg__EnforcedYaw__Sequence__init(rm_interface__msg__EnforcedYaw__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__EnforcedYaw * data = NULL;

  if (size) {
    data = (rm_interface__msg__EnforcedYaw *)allocator.zero_allocate(size, sizeof(rm_interface__msg__EnforcedYaw), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rm_interface__msg__EnforcedYaw__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rm_interface__msg__EnforcedYaw__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
rm_interface__msg__EnforcedYaw__Sequence__fini(rm_interface__msg__EnforcedYaw__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      rm_interface__msg__EnforcedYaw__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

rm_interface__msg__EnforcedYaw__Sequence *
rm_interface__msg__EnforcedYaw__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__EnforcedYaw__Sequence * array = (rm_interface__msg__EnforcedYaw__Sequence *)allocator.allocate(sizeof(rm_interface__msg__EnforcedYaw__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rm_interface__msg__EnforcedYaw__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rm_interface__msg__EnforcedYaw__Sequence__destroy(rm_interface__msg__EnforcedYaw__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rm_interface__msg__EnforcedYaw__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rm_interface__msg__EnforcedYaw__Sequence__are_equal(const rm_interface__msg__EnforcedYaw__Sequence * lhs, const rm_interface__msg__EnforcedYaw__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rm_interface__msg__EnforcedYaw__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rm_interface__msg__EnforcedYaw__Sequence__copy(
  const rm_interface__msg__EnforcedYaw__Sequence * input,
  rm_interface__msg__EnforcedYaw__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rm_interface__msg__EnforcedYaw);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rm_interface__msg__EnforcedYaw * data =
      (rm_interface__msg__EnforcedYaw *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rm_interface__msg__EnforcedYaw__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rm_interface__msg__EnforcedYaw__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rm_interface__msg__EnforcedYaw__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
