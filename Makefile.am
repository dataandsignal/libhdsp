#SUBDIRS = test
dist_doc_DATA = README.txt

AM_CFLAGS    = -I./src -Iinclude -I$(srcdir)/include
lib_LTLIBRARIES = libhdsp.la
libhdsp_la_SOURCES = src/hdsp.c
include_HEADERS = include/hdsp.h
libhdsp_la_LDFLAGS = -version-info 1:0:0

#LIBS += -lhdsp

bin_PROGRAMS = lhdsptool
lhdsptool_SOURCES = test/hdsptool.c
lhdsptool_LDADD = libhdsp.la

check_PROGRAMS = test1 test2 test3 test4 test5
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

