#SUBDIRS = test
dist_doc_DATA = README.txt

AM_CFLAGS    = -I./src -Iinclude -I$(srcdir)/include
lib_LTLIBRARIES = libhdsp.la
libhdsp_la_SOURCES = src/hdsp.c
include_HEADERS = include/hdsp.h
libhdsp_la_LDFLAGS = -version-info 1:0:0

#LIBS += -lhdsp

bin_PROGRAMS = hdsptool
hdsptool_SOURCES = test/hdsptool.c
hdsptool_LDADD = libhdsp.la

check_PROGRAMS = test1 test2 test3 test4 test5 test6 test7 test8
TESTS = $(check_PROGRAMS)

test1_SOURCES = test/test1.c
test1_CFLAGS = -Iinclude
test1_LDADD = libhdsp.la

test2_SOURCES = test/test2.c
test2_CFLAGS = -Iinclude
test2_LDADD = libhdsp.la

test3_SOURCES = test/test3.c
test3_CFLAGS = -Iinclude
test3_LDADD = libhdsp.la

test4_SOURCES = test/test4.c
test4_CFLAGS = -Iinclude
test4_LDADD = libhdsp.la

test5_SOURCES = test/test5.c
test5_CFLAGS = -Iinclude
test5_LDADD = libhdsp.la

test6_SOURCES = test/test6.c
test6_CFLAGS = -Iinclude
test6_LDADD = libhdsp.la

test7_SOURCES = test/test7.c
test7_CFLAGS = -Iinclude
test7_LDADD = libhdsp.la

test8_SOURCES = test/test8.c
test8_CFLAGS = -Iinclude
test8_LDADD = libhdsp.la

