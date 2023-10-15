#!/bin/sh
# Run this file to set up the build system: configure, makefiles, etc.
set -e
echo "Updating build configuration files for libhdsp, please wait...."

autoreconf -isf
