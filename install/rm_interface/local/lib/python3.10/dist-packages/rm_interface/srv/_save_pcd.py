# generated from rosidl_generator_py/resource/_idl.py.em
# with input from rm_interface:srv/SavePCD.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_SavePCD_Request(type):
    """Metaclass of message 'SavePCD_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('rm_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'rm_interface.srv.SavePCD_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__save_pcd__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__save_pcd__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__save_pcd__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__save_pcd__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__save_pcd__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SavePCD_Request(metaclass=Metaclass_SavePCD_Request):
    """Message class 'SavePCD_Request'."""

    __slots__ = [
        '_file_path',
    ]

    _fields_and_field_types = {
        'file_path': 'string',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.file_path = kwargs.get('file_path', str())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.file_path != other.file_path:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def file_path(self):
        """Message field 'file_path'."""
        return self._file_path

    @file_path.setter
    def file_path(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'file_path' field must be of type 'str'"
        self._file_path = value


# Import statements for member types

# already imported above
# import builtins

import math  # noqa: E402, I100

# already imported above
# import rosidl_parser.definition


class Metaclass_SavePCD_Response(type):
    """Metaclass of message 'SavePCD_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('rm_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'rm_interface.srv.SavePCD_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__save_pcd__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__save_pcd__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__save_pcd__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__save_pcd__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__save_pcd__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SavePCD_Response(metaclass=Metaclass_SavePCD_Response):
    """Message class 'SavePCD_Response'."""

    __slots__ = [
        '_memory_gb',
        '_point_num',
        '_result',
    ]

    _fields_and_field_types = {
        'memory_gb': 'float',
        'point_num': 'int32',
        'result': 'int32',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.memory_gb = kwargs.get('memory_gb', float())
        self.point_num = kwargs.get('point_num', int())
        self.result = kwargs.get('result', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.memory_gb != other.memory_gb:
            return False
        if self.point_num != other.point_num:
            return False
        if self.result != other.result:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def memory_gb(self):
        """Message field 'memory_gb'."""
        return self._memory_gb

    @memory_gb.setter
    def memory_gb(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'memory_gb' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'memory_gb' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._memory_gb = value

    @builtins.property
    def point_num(self):
        """Message field 'point_num'."""
        return self._point_num

    @point_num.setter
    def point_num(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'point_num' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'point_num' field must be an integer in [-2147483648, 2147483647]"
        self._point_num = value

    @builtins.property
    def result(self):
        """Message field 'result'."""
        return self._result

    @result.setter
    def result(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'result' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'result' field must be an integer in [-2147483648, 2147483647]"
        self._result = value


class Metaclass_SavePCD(type):
    """Metaclass of service 'SavePCD'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('rm_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'rm_interface.srv.SavePCD')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__save_pcd

            from rm_interface.srv import _save_pcd
            if _save_pcd.Metaclass_SavePCD_Request._TYPE_SUPPORT is None:
                _save_pcd.Metaclass_SavePCD_Request.__import_type_support__()
            if _save_pcd.Metaclass_SavePCD_Response._TYPE_SUPPORT is None:
                _save_pcd.Metaclass_SavePCD_Response.__import_type_support__()


class SavePCD(metaclass=Metaclass_SavePCD):
    from rm_interface.srv._save_pcd import SavePCD_Request as Request
    from rm_interface.srv._save_pcd import SavePCD_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
