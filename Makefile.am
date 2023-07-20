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

check_PROGRAMS = test1
TESTS = $(check_PROGRAMS)

test1_SOURCES = test/test1.c src/hdsp.c include/hdsp.h
test1_CFLAGS = -Iinclude

