// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rm_interface:msg/LocalState.idl
// generated code does not contain a copyright notice
#include "rm_interface/msg/detail/local_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rm_interface__msg__LocalState__init(rm_interface__msg__LocalState * msg)
{
  if (!msg) {
    return false;
  }
  // result
  // safe_state
  return true;
}

void
rm_interface__msg__LocalState__fini(rm_interface__msg__LocalState * msg)
{
  if (!msg) {
    return;
  }
  // result
  // safe_state
}

bool
rm_interface__msg__LocalState__are_equal(const rm_interface__msg__LocalState * lhs, const rm_interface__msg__LocalState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // result
  if (lhs->result != rhs->result) {
    return false;
  }
  // safe_state
  if (lhs->safe_state != rhs->safe_state) {
    return false;
  }
  return true;
}

bool
rm_interface__msg__LocalState__copy(
  const rm_interface__msg__LocalState * input,
  rm_interface__msg__LocalState * output)
{
  if (!input || !output) {
    return false;
  }
  // result
  output->result = input->result;
  // safe_state
  output->safe_state = input->safe_state;
  return true;
}

rm_interface__msg__LocalState *
rm_interface__msg__LocalState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__LocalState * msg = (rm_interface__msg__LocalState *)allocator.allocate(sizeof(rm_interface__msg__LocalState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rm_interface__msg__LocalState));
  bool success = rm_interface__msg__LocalState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rm_interface__msg__LocalState__destroy(rm_interface__msg__LocalState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rm_interface__msg__LocalState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rm_interface__msg__LocalState__Sequence__init(rm_interface__msg__LocalState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__LocalState * data = NULL;

  if (size) {
    data = (rm_interface__msg__LocalState *)allocator.zero_allocate(size, sizeof(rm_interface__msg__LocalState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rm_interface__msg__LocalState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rm_interface__msg__LocalState__fini(&data[i - 1]);
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
rm_interface__msg__LocalState__Sequence__fini(rm_interface__msg__LocalState__Sequence * array)
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
      rm_interface__msg__LocalState__fini(&array->data[i]);
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

rm_interface__msg__LocalState__Sequence *
rm_interface__msg__LocalState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_interface__msg__LocalState__Sequence * array = (rm_interface__msg__LocalState__Sequence *)allocator.allocate(sizeof(rm_interface__msg__LocalState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rm_interface__msg__LocalState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rm_interface__msg__LocalState__Sequence__destroy(rm_interface__msg__LocalState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rm_interface__msg__LocalState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rm_interface__msg__LocalState__Sequence__are_equal(const rm_interface__msg__LocalState__Sequence * lhs, const rm_interface__msg__LocalState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rm_interface__msg__LocalState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rm_interface__msg__LocalState__Sequence__copy(
  const rm_interface__msg__LocalState__Sequence * input,
  rm_interface__msg__LocalState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rm_interface__msg__LocalState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rm_interface__msg__LocalState * data =
      (rm_interface__msg__LocalState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rm_interface__msg__LocalState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rm_interface__msg__LocalState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rm_interface__msg__LocalState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
