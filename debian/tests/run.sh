#!/bin/bash

set -e

AUTOPKGTEST_SRC=$PWD
STEM=$1

cd $AUTOPKGTEST_TMP
cp $AUTOPKGTEST_SRC/debian/tests/{$STEM.cpp,Makefile} .
make $STEM

./$STEM
