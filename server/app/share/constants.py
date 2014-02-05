#coding:utf8

# command id
COMMAND_FORWARD = 0
COMMAND_TEST = 1
COMMAND_TEST_PUSH = 2
COMMAND_LOGIN = 101

# error code
E_OK = 0
E_UNKNOWN = 1
E_WRONG_PASSWORD = 2
E_BLOCKED = 3
E_DB_ERROR = 4
E_NOT_LOGGED_IN = 5

# dict keys
KEY_ERRNO = 'errno'
KEY_ERRMSG = 'errmsg'
KEY_DATA = 'data'
KEY_USERNAME = 'username'
KEY_PASSWORD = 'password'
KEY_MESSAGE = 'message'