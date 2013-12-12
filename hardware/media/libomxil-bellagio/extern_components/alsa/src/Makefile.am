lib_LTLIBRARIES = libomxalsa.la

libomxalsa_la_SOURCES = omx_alsasink_component.c \
                       omx_alsasrc_component.c \
                       omx_alsasink_component.h \
                       omx_alsasrc_component.h \
                       library_entry_point.c

libomxalsa_la_LIBADD  = -lomxil-bellagio
libomxalsa_la_LDFLAGS = $(ALSA_LIBS)
libomxalsa_la_CFLAGS  = $(ALSA_CFLAGS) $(OMXIL_CFLAGS)

