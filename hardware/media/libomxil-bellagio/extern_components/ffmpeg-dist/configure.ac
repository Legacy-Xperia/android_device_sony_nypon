AC_INIT([OpenMAX Bellagio FFmpeg based Component ], [0.1], [], [libomxffmpegdist])
AM_INIT_AUTOMAKE([tar-ustar])

# Prerequisite autoconf version
AC_PREREQ([2.59])

PKG_CHECK_MODULES(OMXIL, libomxil-bellagio >= 0.9 )
AC_SUBST(OMXIL_CFLAGS)
AC_SUBST(OMXIL_LIBS)

AC_CONFIG_HEADERS([config.h])
CFLAGS="${CFLAGS} -Werror"
AC_PREFIX_DEFAULT(/usr/local/lib/extern_omxcomp)

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
    m4/Makefile
])

################################################################################
# Define the extra arguments the user can pass to the configure script         #
################################################################################

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

# Check for POSIX thread support
ACX_PTHREAD([
                LIBS="$LIBS $PTHREAD_LIBS"
                CFLAGS="$CFLAGS $PTHREAD_CFLAGS -g -Wall"
                CC="$PTHREAD_CC"
                AC_SUBST([LIBS])
                AC_SUBST([CFLAGS])
                AC_SUBST([CC])
            ],
            [AC_MSG_ERROR([POSIX threads support is required])])

# Check for libdl
AC_SEARCH_LIBS([dlopen], [dl], [], [AC_MSG_ERROR([libdl is required])])

# Define components default ldflags (man ld)
PLUGIN_LDFLAGS="-module -avoid-version -no-undefined -as-needed"
AC_SUBST(PLUGIN_LDFLAGS)

################################################################################
# Check for header files                                                       #
################################################################################

# Check if OpenMAX header are present
if test "x$omxcore_files" = "xyes"; then
	AC_CHECK_HEADER([OMX_Core.h], [with_omxcore_files=yes], [with_omxcore_files=no])
fi

################################################################################
# Check for pkg-config modules                                                 #
################################################################################

# unused
# Check for FFmpeg component dependencies
        PKG_CHECK_MODULES([FFMPEG], [libavcodec libavformat libavutil libswscale], [with_ffmpegdist=yes], [with_ffmpegdist=no])

# Check for FFmpeg API version
if test "x$with_ffmpegdist" = "xyes"; then
        old_CPPFLAGS="${CPPFLAGS}"
        old_LDFLAGS="${LDFLAGS}"
        CPPFLAGS="${CPPFLAGS} ${FFMPEG_CFLAGS}"
        LDFLAGS="${LDFLAGS} ${FFMPEG_LIBS}"

        # Check for FFmpeg audio decoder API version
        AC_CHECK_LIB([avcodec], [avcodec_decode_audio2],
                     [AC_DEFINE([FFMPEG_DECODER_VERSION], [2], [FFmpeg audio2 decoder API])],
                     [AC_DEFINE([FFMPEG_DECODER_VERSION], [1], [FFmpeg audio deocder API ])],
                     [-lavutil -lm])

        # Check for location of FFmpeg library headers. These were moved
        # in FFmpeg r12194 (2008-02-25). For example, instead of
        # including <ffmpeg/avformat.h>, now include <libavformat/avformat.h>.
        AC_CHECK_HEADER([libavformat/avformat.h],
                        [AC_DEFINE([FFMPEG_LIBNAME_HEADERS], [1],
                         [FFmpeg has separate named include directories for each of its libraries])])

        # Restore flags
        CPPFLAGS="${old_CPPFLAGS}"
        LDFLAGS="${old_LDFLAGS}"
fi

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
AM_CONDITIONAL([WITH_FFMPEGCOMPONENTS], [test x$with_ffmpegdist = xyes && test x$with_omxcore_files = xyes])

AC_OUTPUT
