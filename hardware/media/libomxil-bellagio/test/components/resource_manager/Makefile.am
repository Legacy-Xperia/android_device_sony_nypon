check_PROGRAMS = omxrmtest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir)

omxrmtest_SOURCES = omxrmtest.c omxrmtest.h
omxrmtest_LDADD = $(bellagio_LDADD) -lpthread
omxrmtest_CFLAGS = $(common_CFLAGS)

