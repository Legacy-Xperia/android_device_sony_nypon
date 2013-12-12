lib_LTLIBRARIES = libomxmad.la

libomxmad_la_SOURCES = omx_maddec_component.c  \
                       omx_maddec_component.h \
                       library_entry_point.c

libomxmad_la_LIBADD  = -lomxil-bellagio
libomxmad_la_LDFLAGS = $(MAD_LIBS)
libomxmad_la_CFLAGS  = $(OMXIL_CFLAGS)

