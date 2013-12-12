noinst_LTLIBRARIES = libomxaudio_reader.la

libomxaudio_reader_la_SOURCES = audio_reader.c audio_reader.h

libomxaudio_reader_la_LDFLAGS = -lomxil-bellagio
libomxaudio_reader_la_CFLAGS = $(OMXIL_CFLAGS)

bin_PROGRAMS = audio_reader_test

audio_reader_test_SOURCES = audio_reader_test.c audio_reader_test.h
audio_reader_test_LDADD = -lpthread -lomxil-bellagio
audio_reader_test_CFLAGS = $(OMXIL_CFLAGS)
