lib_LTLIBRARIES = libomxffmpegdist.la

libomxffmpegdist_la_SOURCES = omx_audioenc_component.c  \
                          omx_filereader_component.c \
                          omx_videodec_component.c \
                          omx_audiodec_component.c  \
                          omx_ffmpeg_colorconv_component.c \
                          omx_parser3gp_component.c \
                          omx_videoenc_component.c \
                          omx_audioenc_component.h \
                          omx_filereader_component.h \
                          omx_videodec_component.h \
                          omx_audiodec_component.h  \
                          omx_ffmpeg_colorconv_component.h \
                          omx_parser3gp_component.h \
                          omx_videoenc_component.h \
                          library_entry_point.c

libomxffmpegdist_la_LIBADD  = -lomxil-bellagio
libomxffmpegdist_la_LDFLAGS = $(FFMPEG_LIBS)
libomxffmpegdist_la_CFLAGS  = $(FFMPEG_CFLAGS) $(OMXIL_CFLAGS)

