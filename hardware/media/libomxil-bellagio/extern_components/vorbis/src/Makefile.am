lib_LTLIBRARIES = libomxvorbis.la

libomxvorbis_la_SOURCES = omx_vorbisdec_component.c  \
                          omx_vorbisdec_component.h \
                          library_entry_point.c

libomxvorbis_la_LIBADD  = -lomxil-bellagio
libomxvorbis_la_LDFLAGS = $(VORBIS_LIBS)
libomxvorbis_la_CFLAGS  = $(OMXIL_CFLAGS)

