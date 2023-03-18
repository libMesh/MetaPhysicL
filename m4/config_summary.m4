# SYNOPSIS
#
#   Summarizes configuration settings.
#
#   AX_SUMMARIZE_CONFIG([, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
#
# DESCRIPTION
#
#   Outputs a summary of relevant configuration settings.
#
# LAST MODIFICATION
#
#   2010-03-24
#

AC_DEFUN([AX_SUMMARIZE_CONFIG],
[

echo
echo '----------------------------------- SUMMARY -----------------------------------'
echo
echo Package version............... : $PACKAGE-$VERSION
echo
echo C++ compiler.................. : $CXX
echo C++ compiler flags............ : $CXXFLAGS
AS_ECHO(["Any warnings-to-errors flags....... : $ACSM_ANY_WERROR_FLAG"])
AS_ECHO(["Any extra paranoid warning flags... : $ACSM_ANY_PARANOID_FLAGS"])
echo Install dir................... : $prefix
echo Build user.................... : $USER
echo Build host.................... : $BUILD_HOST
echo Configure date................ : $BUILD_DATE
echo Build architecture............ : $BUILD_ARCH
echo Git revision number........... : $BUILD_VERSION
echo
echo Optional Packages for Testing:
if test "x$HAVE_MASA" = "x1"; then
  echo '  'MASA........................ : yes
  echo '  'MASA_INCLUDE ............... : $MASA_INCLUDE
  echo '  'MASA_LIB ................... : $MASA_LIB
else
  echo '  'MASA........................ : no
fi
if test "x$HAVE_VEXCL" = "x1"; then
  echo '  'VEXCL....................... : yes
  echo '  'VEXCL_CPPFLAGS.............. : $VEXCL_CPPFLAGS
  echo '  'VEXCL_LDFLAGS............... : $VEXCL_LDFLAGS
  echo '  'VEXCL_LIBS.................. : $VEXCL_LIBS
else
  echo '  'VEXCL....................... : no
fi
if test "x$HAVE_TIMPI" = "x1"; then
  echo '  'TIMPI....................... : yes
  echo '  'TIMPI_CPPFLAGS.............. : $TIMPI_CPPFLAGS
  if test "x$HAVE_TIMPI_LIB" = "x1"; then
    echo '  'TIMPI_LIB................... : yes
  else
    echo '  'TIMPI_LIB................... : no
  fi
  echo '  'TIMPI_LIBS.................. : $TIMPI_LIBS
else
  echo '  'TIMPI....................... : no
fi
echo
echo '-------------------------------------------------------------------------------'

echo
echo Configure complete, now type \'make\' and then \'make install\'.
echo

])
