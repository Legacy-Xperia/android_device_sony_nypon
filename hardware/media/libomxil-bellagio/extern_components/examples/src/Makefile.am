bin_PROGRAMS = omxaudiodectest omxvoicexchange omxaudioenctest \
               omxcameratest omxmuxtest omxparsertest \
               omxvideocapnplay omxvideoenctest omxvideodectest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir) $(OMXIL_CFLAGS)

omxaudiodectest_SOURCES = omxaudiodectest.c omxaudiodectest.h
omxaudiodectest_LDADD = $(bellagio_LDADD) -lpthread
omxaudiodectest_CFLAGS = $(common_CFLAGS)

omxvoicexchange_SOURCES = omxvoicexchange.c omxvoicexchange.h 
omxvoicexchange_LDADD =  $(bellagio_LDADD) -lpthread 
omxvoicexchange_CFLAGS= $(common_CFLAGS)

omxaudioenctest_SOURCES = omxaudioenctest.c omxaudioenctest.h
omxaudioenctest_LDADD = $(bellagio_LDADD) -lpthread
omxaudioenctest_CFLAGS = $(common_CFLAGS)


omxcameratest_SOURCES = omxcameratest.c omxcameratest.h
omxcameratest_LDADD = $(bellagio_LDADD) -lpthread
omxcameratest_CFLAGS = $(common_CFLAGS)


omxmuxtest_SOURCES = omxmuxtest.c omxmuxtest.h
omxmuxtest_LDADD = $(bellagio_LDADD) -lpthread
omxmuxtest_CFLAGS = $(common_CFLAGS)


omxparsertest_SOURCES = omxparsertest.c omxparsertest.h
omxparsertest_LDADD = $(bellagio_LDADD) -lpthread
omxparsertest_CFLAGS = $(common_CFLAGS)


omxvideocapnplay_SOURCES = omxvideocapnplay.c omxvideocapnplay.h
omxvideocapnplay_LDADD   = $(bellagio_LDADD) -lpthread
omxvideocapnplay_CFLAGS  = $(common_CFLAGS)

omxvideodectest_SOURCES = omxvideodectest.c omxvideodectest.h
omxvideodectest_LDADD   = $(bellagio_LDADD) -lpthread
omxvideodectest_CFLAGS  = $(common_CFLAGS)

omxvideoenctest_SOURCES = omxvideoenctest.c omxvideoenctest.h
omxvideoenctest_LDADD   = $(bellagio_LDADD) -lpthread
omxvideoenctest_CFLAGS  = $(common_CFLAGS)
