lib_LTLIBRARIES = libomximagejpeg.la

libomximagejpeg_la_SOURCES = omx_jpegdec_component.c \
                             omx_jpegenc_component.c \
                             library_entry_point.c \
                             wrbmp.c \
                             rdbmp.c \
                             cderror.h \
                             cdjpeg.h \
                             jinclude.h \
                             omx_jpegdec_component.h \
                             omx_jpegenc_component.h

libomximagejpeg_la_LIBADD  = -lomxil-bellagio  -ljpeg
libomximagejpeg_la_LDFLAGS =
libomximagejpeg_la_CFLAGS = $(OMXIL_CFLAGS)
