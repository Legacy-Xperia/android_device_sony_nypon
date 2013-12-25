check_PROGRAMS = omxvideocapturetest 

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS  = -I$(includedir)

omxvideocapturetest_SOURCES = omxvideocapturetest.c omxvideocapturetest.h
omxvideocapturetest_LDADD   = $(bellagio_LDADD)
omxvideocapturetest_CFLAGS  = $(common_CFLAGS) $(OMXIL_CFLAGS)

