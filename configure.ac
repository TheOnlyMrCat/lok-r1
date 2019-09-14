#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Lok compiler], [0.1-alpha])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/parse.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX(11)
AC_PROG_CC
AC_PROG_LEX

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

# Checks for boost
AX_BOOST_BASE([1.60])
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM

AC_OUTPUT
