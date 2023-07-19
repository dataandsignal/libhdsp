#SUBDIRS = test
dist_doc_DATA = README.txt

AM_CFLAGS    = -I./src -Iinclude -I$(srcdir)/include
lib_LTLIBRARIES = libhdsp.la
libhdsp_la_SOURCES = src/lhdsp.c
include_HEADERS = include/lhdsp.h
libhdsp_la_LDFLAGS = -version-info 1:0:0

bin_PROGRAMS = lhdsptool
lhdsptool_SOURCES = src/lhdsptool.c
lhdsptool_LDADD = -lhdsp

check_PROGRAMS = test1
TESTS = $(check_PROGRAMS)

