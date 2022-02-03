#!/bin/bash

set -e

AUTOPKGTEST_SRC=$PWD
STEM=$1

cd $AUTOPKGTEST_TMP
cp $AUTOPKGTEST_SRC/debian/tests/{$STEM.cpp,Makefile} .

# Avoid test failures due to compiler warnings.
make $STEM 2>&1

./$STEM
