check_PROGRAMS = omxaudiocapnplay

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir) $(OMXIL_CFLAGS)

omxaudiocapnplay_SOURCES = omxaudiocapnplay.c omxaudiocapnplay.h
omxaudiocapnplay_LDADD = $(bellagio_LDADD) -lpthread
omxaudiocapnplay_CFLAGS = $(common_CFLAGS)
