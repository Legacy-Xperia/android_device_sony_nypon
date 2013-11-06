check_PROGRAMS = omxjpegdectest omxjpegenctest

bellagio_LDADD = -lomxil-bellagio -lpthread
common_CFLAGS  = -I$(includedir) $(OMXIL_CFLAGS)

omxjpegdectest_SOURCES = omxjpegdectest.c omxjpegdectest.h
omxjpegdectest_LDADD = $(bellagio_LDADD)
omxjpegdectest_CFLAGS = $(common_CFLAGS)

omxjpegenctest_SOURCES = omxjpegenctest.c omxjpegenctest.h
omxjpegenctest_LDADD = $(bellagio_LDADD)
omxjpegenctest_CFLAGS = $(common_CFLAGS)
