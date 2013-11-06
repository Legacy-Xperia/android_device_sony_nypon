SUBDIRS = audio_capturer audio_decoder  audio_reader  audio_renderer \
          audio_writer

lib_LTLIBRARIES = libomxaudioset.la
libomxaudioset_la_SOURCES = library_entry_point.c
libomxaudioset_la_LIBADD = audio_capturer/libomxaudio_capturer.la \
                           audio_decoder/libomxaudio_decoder.la \
                           audio_reader/libomxaudio_reader.la \
                           audio_renderer/libomxaudio_renderer.la \
                           audio_writer/libomxaudio_writer.la

libomxaudioset_la_LDFLAGS = -lomxil-bellagio
libomxaudioset_la_CFLAGS = $(OMXIL_CFLAGS)
