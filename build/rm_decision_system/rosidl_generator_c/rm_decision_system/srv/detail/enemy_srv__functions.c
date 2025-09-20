// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rm_decision_system:srv/EnemySrv.idl
// generated code does not contain a copyright notice
#include "rm_decision_system/srv/detail/enemy_srv__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
rm_decision_system__srv__EnemySrv_Request__init(rm_decision_system__srv__EnemySrv_Request * msg)
{
  if (!msg) {
    return false;
  }
  // x1
  // y1
  // z1
  // x2
  // y2
  // z2
  // enemy_count
  return true;
}

void
rm_decision_system__srv__EnemySrv_Request__fini(rm_decision_system__srv__EnemySrv_Request * msg)
{
  if (!msg) {
    return;
  }
  // x1
  // y1
  // z1
  // x2
  // y2
  // z2
  // enemy_count
}

bool
rm_decision_system__srv__EnemySrv_Request__are_equal(const rm_decision_system__srv__EnemySrv_Request * lhs, const rm_decision_system__srv__EnemySrv_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // x1
  if (lhs->x1 != rhs->x1) {
    return false;
  }
  // y1
  if (lhs->y1 != rhs->y1) {
    return false;
  }
  // z1
  if (lhs->z1 != rhs->z1) {
    return false;
  }
  // x2
  if (lhs->x2 != rhs->x2) {
    return false;
  }
  // y2
  if (lhs->y2 != rhs->y2) {
    return false;
  }
  // z2
  if (lhs->z2 != rhs->z2) {
    return false;
  }
  // enemy_count
  if (lhs->enemy_count != rhs->enemy_count) {
    return false;
  }
  return true;
}

bool
rm_decision_system__srv__EnemySrv_Request__copy(
  const rm_decision_system__srv__EnemySrv_Request * input,
  rm_decision_system__srv__EnemySrv_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // x1
  output->x1 = input->x1;
  // y1
  output->y1 = input->y1;
  // z1
  output->z1 = input->z1;
  // x2
  output->x2 = input->x2;
  // y2
  output->y2 = input->y2;
  // z2
  output->z2 = input->z2;
  // enemy_count
  output->enemy_count = input->enemy_count;
  return true;
}

rm_decision_system__srv__EnemySrv_Request *
rm_decision_system__srv__EnemySrv_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Request * msg = (rm_decision_system__srv__EnemySrv_Request *)allocator.allocate(sizeof(rm_decision_system__srv__EnemySrv_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rm_decision_system__srv__EnemySrv_Request));
  bool success = rm_decision_system__srv__EnemySrv_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rm_decision_system__srv__EnemySrv_Request__destroy(rm_decision_system__srv__EnemySrv_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rm_decision_system__srv__EnemySrv_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rm_decision_system__srv__EnemySrv_Request__Sequence__init(rm_decision_system__srv__EnemySrv_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Request * data = NULL;

  if (size) {
    data = (rm_decision_system__srv__EnemySrv_Request *)allocator.zero_allocate(size, sizeof(rm_decision_system__srv__EnemySrv_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rm_decision_system__srv__EnemySrv_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rm_decision_system__srv__EnemySrv_Request__fini(&data[i - 1]);
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
rm_decision_system__srv__EnemySrv_Request__Sequence__fini(rm_decision_system__srv__EnemySrv_Request__Sequence * array)
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
      rm_decision_system__srv__EnemySrv_Request__fini(&array->data[i]);
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

rm_decision_system__srv__EnemySrv_Request__Sequence *
rm_decision_system__srv__EnemySrv_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Request__Sequence * array = (rm_decision_system__srv__EnemySrv_Request__Sequence *)allocator.allocate(sizeof(rm_decision_system__srv__EnemySrv_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rm_decision_system__srv__EnemySrv_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rm_decision_system__srv__EnemySrv_Request__Sequence__destroy(rm_decision_system__srv__EnemySrv_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rm_decision_system__srv__EnemySrv_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rm_decision_system__srv__EnemySrv_Request__Sequence__are_equal(const rm_decision_system__srv__EnemySrv_Request__Sequence * lhs, const rm_decision_system__srv__EnemySrv_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rm_decision_system__srv__EnemySrv_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rm_decision_system__srv__EnemySrv_Request__Sequence__copy(
  const rm_decision_system__srv__EnemySrv_Request__Sequence * input,
  rm_decision_system__srv__EnemySrv_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rm_decision_system__srv__EnemySrv_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rm_decision_system__srv__EnemySrv_Request * data =
      (rm_decision_system__srv__EnemySrv_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rm_decision_system__srv__EnemySrv_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rm_decision_system__srv__EnemySrv_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rm_decision_system__srv__EnemySrv_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
rm_decision_system__srv__EnemySrv_Response__init(rm_decision_system__srv__EnemySrv_Response * msg)
{
  if (!msg) {
    return false;
  }
  // result
  return true;
}

void
rm_decision_system__srv__EnemySrv_Response__fini(rm_decision_system__srv__EnemySrv_Response * msg)
{
  if (!msg) {
    return;
  }
  // result
}

bool
rm_decision_system__srv__EnemySrv_Response__are_equal(const rm_decision_system__srv__EnemySrv_Response * lhs, const rm_decision_system__srv__EnemySrv_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // result
  if (lhs->result != rhs->result) {
    return false;
  }
  return true;
}

bool
rm_decision_system__srv__EnemySrv_Response__copy(
  const rm_decision_system__srv__EnemySrv_Response * input,
  rm_decision_system__srv__EnemySrv_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // result
  output->result = input->result;
  return true;
}

rm_decision_system__srv__EnemySrv_Response *
rm_decision_system__srv__EnemySrv_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Response * msg = (rm_decision_system__srv__EnemySrv_Response *)allocator.allocate(sizeof(rm_decision_system__srv__EnemySrv_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rm_decision_system__srv__EnemySrv_Response));
  bool success = rm_decision_system__srv__EnemySrv_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rm_decision_system__srv__EnemySrv_Response__destroy(rm_decision_system__srv__EnemySrv_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rm_decision_system__srv__EnemySrv_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rm_decision_system__srv__EnemySrv_Response__Sequence__init(rm_decision_system__srv__EnemySrv_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Response * data = NULL;

  if (size) {
    data = (rm_decision_system__srv__EnemySrv_Response *)allocator.zero_allocate(size, sizeof(rm_decision_system__srv__EnemySrv_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rm_decision_system__srv__EnemySrv_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rm_decision_system__srv__EnemySrv_Response__fini(&data[i - 1]);
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
rm_decision_system__srv__EnemySrv_Response__Sequence__fini(rm_decision_system__srv__EnemySrv_Response__Sequence * array)
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
      rm_decision_system__srv__EnemySrv_Response__fini(&array->data[i]);
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

rm_decision_system__srv__EnemySrv_Response__Sequence *
rm_decision_system__srv__EnemySrv_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rm_decision_system__srv__EnemySrv_Response__Sequence * array = (rm_decision_system__srv__EnemySrv_Response__Sequence *)allocator.allocate(sizeof(rm_decision_system__srv__EnemySrv_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rm_decision_system__srv__EnemySrv_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rm_decision_system__srv__EnemySrv_Response__Sequence__destroy(rm_decision_system__srv__EnemySrv_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rm_decision_system__srv__EnemySrv_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rm_decision_system__srv__EnemySrv_Response__Sequence__are_equal(const rm_decision_system__srv__EnemySrv_Response__Sequence * lhs, const rm_decision_system__srv__EnemySrv_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rm_decision_system__srv__EnemySrv_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rm_decision_system__srv__EnemySrv_Response__Sequence__copy(
  const rm_decision_system__srv__EnemySrv_Response__Sequence * input,
  rm_decision_system__srv__EnemySrv_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rm_decision_system__srv__EnemySrv_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rm_decision_system__srv__EnemySrv_Response * data =
      (rm_decision_system__srv__EnemySrv_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rm_decision_system__srv__EnemySrv_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rm_decision_system__srv__EnemySrv_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rm_decision_system__srv__EnemySrv_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
