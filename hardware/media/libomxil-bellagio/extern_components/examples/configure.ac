AC_INIT([OpenMAX application examples], [0.1], [], [omxBellagioExamples])
AM_INIT_AUTOMAKE([tar-ustar])

# Prerequisite autoconf version
AC_PREREQ([2.59])

PKG_CHECK_MODULES(OMXIL, libomxil-bellagio >= 0.9 )
AC_SUBST(OMXIL_CFLAGS)
AC_SUBST(OMXIL_LIBS)

AC_CONFIG_HEADERS([config.h])
CFLAGS="${CFLAGS} -Werror"

################################################################################
# Set the shared versioning info, according to section 6.3 of the libtool info #
# pages. CURRENT:REVISION:AGE must be updated immediately before each release: #
#                                                                              #
#   * If the library source code has changed at all since the last             #
#     update, then increment REVISION (`C:R:A' becomes `C:r+1:A').             #
#                                                                              #
#   * If any interfaces have been added, removed, or changed since the         #
#     last update, increment CURRENT, and set REVISION to 0.                   #
#                                                                              #
#   * If any interfaces have been added since the last public release,         #
#     then increment AGE.                                                      #
#                                                                              #
#   * If any interfaces have been removed since the last public release,       #
#     then set AGE to 0.                                                       #
#                                                                              #
################################################################################
SHARED_VERSION_INFO="0:0:0"
AC_SUBST(SHARED_VERSION_INFO)

# Set to 'm4' the directory where the extra autoconf macros are stored
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
################################################################################
# Check for programs                                                           #
################################################################################

# Check for a working C compiler
AC_PROG_CC
AM_PROG_CC_C_O

# Check for libtool
AM_PROG_LIBTOOL

################################################################################
# Check for libraries                                                          #
################################################################################

################################################################################
# Check for header files                                                       #
################################################################################

################################################################################
# Check for pkg-config modules                                                 #
################################################################################

################################################################################
# Check for types                                                              #
################################################################################

# unused

################################################################################
# Check for structures                                                         #
################################################################################

# unused

################################################################################
# Check for compiler characteristics                                           #
################################################################################

# unused

################################################################################
# Check for library functions                                                  #
################################################################################

# unused

################################################################################
# Check for system services                                                    #
################################################################################

# unused

################################################################################
# Conditionals and file output                                                 #
################################################################################

AC_OUTPUT
