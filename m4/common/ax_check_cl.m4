# -*- mode: autoconf -*-
#
# AX_CHECK_CL
#
# Check for an OpenCL implementation.  If CL is found, the required
# include, compiler and linker flags are included in the output
# variables "CL_CPPFLAGS", "CL_CFLAGS" and "CL_LIBS", respectively.
# If no usable CL implementation is found, "no_cl" is set to "yes".
#
# If the header "CL/OpenCL.h" is found, "HAVE_CL_OPENCL_H" is defined.  If the header
# "OpenCL/OpenCL.h" is found, HAVE_OPENCL_OPENCL_H is defined.  These preprocessor
# definitions may not be mutually exclusive.
#
# Based on AX_CHECK_GL, version: 2.4 author: Braden McDaniel
# <braden@endoframe.com>
#
# Updated 2013-07-18 by Roy Stogner <roystgnr@ices.utexas.edu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#
# As a special exception, the you may copy, distribute and modify the
# configure scripts that are the output of Autoconf when processing
# the Macro.  You need not follow the terms of the GNU General Public
# License when using or distributing such scripts.
#
AC_DEFUN([AX_CHECK_CL],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_PROG_SED])dnl
AC_REQUIRE([ACX_PTHREAD])dnl

AC_ARG_ENABLE([opencl],
    [AS_HELP_STRING([--disable-opencl],
                    [do not use OpenCL])],
    [enable_opencl=$enableval],
    [enable_opencl='yes'])

if test "$enable_opencl" = 'yes'; then
  AC_LANG_PUSH([$1])
  AX_LANG_COMPILER_MS
  AS_IF([test X$ax_compiler_ms = Xno],
        [CL_CFLAGS="${PTHREAD_CFLAGS}"; CL_LIBS="${PTHREAD_LIBS} -lm"])
  
  ax_save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CL_CFLAGS $CPPFLAGS"
  if test "x${OPENCL_DIR}" != "x" -a -d "${OPENCL_DIR}/include"; then
    OPENCL_INCLUDE="${OPENCL_DIR}/include"
  fi
  if test "x${OPENCL_INCLUDE}" != x; then
    CL_CPPFLAGS="-I$OPENCL_INCLUDE"
    CPPFLAGS="$CPPFLAGS $CL_CPPFLAGS"
  else
    CL_CPPFLAGS=""
  fi

  if test "x${OPENCL_DIR}" != "x" -a -d "${OPENCL_DIR}/lib"; then
    OPENCL_LIBDIR="${OPENCL_DIR}/lib"
  fi
  if test "x${OPENCL_LIB}" != "x" -a -d "${OPENCL_LIB}"; then
    OPENCL_LIBDIR="${OPENCL_LIB}"
  fi
  if test "x${OPENCL_LIBDIR}" != x; then
    CL_LDFLAGS="-L$OPENCL_LIBDIR"
  else
    CL_LDFLAGS=""
  fi

  AC_CHECK_HEADERS([CL/cl.h OpenCL/cl.h])
  CPPFLAGS=$ax_save_CPPFLAGS
  
  AC_CHECK_HEADERS([windows.h])
  
  m4_define([AX_CHECK_CL_PROGRAM],
            [AC_LANG_PROGRAM([[
  # if defined(HAVE_WINDOWS_H) && defined(_WIN32)
  #   include <windows.h>
  # endif
  # ifdef HAVE_CL_CL_H
  #   include <CL/cl.h>
  # elif defined(HAVE_OPENCL_CL_H)
  #   include <OpenCL/cl.h>
  # else
  #   error no CL.h
  # endif]],
                             [[clCreateContextFromType(0,0,0,0,0)]])])
  
  AC_CACHE_CHECK([for OpenCL library], [ax_cv_check_cl_libcl],
  [ax_cv_check_cl_libcl=no
  case $host_cpu in
    x86_64) ax_check_cl_libdir=lib64 ;;
    *)      ax_check_cl_libdir=lib ;;
  esac
  ax_save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CL_CFLAGS $CL_CPPFLAGS $CPPFLAGS"
  ax_save_LIBS=$LIBS
  ax_save_LDFLAGS=$LDFLAGS
  LIBS=""
  LDFLAGS="$CL_LDFLAGS"
  ax_check_libs="-lOpenCL -lCL -lclparser"
  for ax_lib in $ax_check_libs; do
    AS_IF([test X$ax_compiler_ms = Xyes],
          [ax_try_lib=`echo $ax_lib | $SED -e 's/^-l//' -e 's/$/.lib/'`],
          [ax_try_lib=$ax_lib])
    LIBS="$ax_try_lib $CL_LIBS $ax_save_LIBS"
    AC_LINK_IFELSE([AX_CHECK_CL_PROGRAM],
                   [ax_cv_check_cl_libcl=$ax_try_lib; break],
                   [ax_check_cl_nvidia_flags="-L/usr/$ax_check_cl_libdir/nvidia" LIBS="$ax_try_lib $ax_check_cl_nvidia_flags $CL_LIBS $ax_save_LIBS"
                   AC_LINK_IFELSE([AX_CHECK_CL_PROGRAM],
                                  [ax_cv_check_cl_libcl="$ax_try_lib $ax_check_cl_nvidia_flags"; break],
                                  [ax_check_cl_dylib_flag='-framework OpenCL' LIBS="$ax_check_cl_dylib_flag"
                                  AC_LINK_IFELSE([AX_CHECK_CL_PROGRAM],
                                                 [ax_cv_check_cl_libcl="$ax_check_cl_dylib_flag"; break])])])
  done
  
  AS_IF([test "X$ax_cv_check_cl_libcl" = Xno -a X$no_x = Xyes],
        [LIBS='-framework OpenCL'
        AC_LINK_IFELSE([AX_CHECK_CL_PROGRAM],
                       [ax_cv_check_cl_libcl=$LIBS])])
  
  LIBS=$ax_save_LIBS
  LDFLAGS=$ax_save_LDFLAGS
  CPPFLAGS=$ax_save_CPPFLAGS])
  
  AS_IF([test "X$ax_cv_check_cl_libcl" = Xno],
        [no_cl=yes; CL_CFLAGS=""; CL_LIBS=""],
        [CL_LIBS="$ax_cv_check_cl_libcl $CL_LIBS"; AC_DEFINE([_OPENCL], [1],
      [Define this for the OpenCL Accelerator])])
  AC_LANG_POP([$1])
fi
  
AC_SUBST([CL_CPPFLAGS])
AC_SUBST([CL_CFLAGS])
AC_SUBST([CL_LDFLAGS])
AC_SUBST([CL_LIBS])
])dnl
