#!/bin/sh

set -x

CXX=g++
CXXFLAGS="-std=c++11 -Wall -g"
OUT=bin

mkdir -p $OUT

$CXX $CXXFLAGS list00.cc -o $OUT/list00
$CXX $CXXFLAGS list00.cc -m32 -o $OUT/list00m32

$CXX $CXXFLAGS list01.cc -o $OUT/list01
$CXX $CXXFLAGS list01err0.cc -o $OUT/list01err0
$CXX $CXXFLAGS -fsanitize=address list01err0.cc -o $OUT/list01err0asan
$CXX $CXXFLAGS list01err0hack.cc -o $OUT/list01err0hack
$CXX $CXXFLAGS list01err1.cc -o $OUT/list01err1
$CXX $CXXFLAGS list01alt.cc -o $OUT/list01alt
$CXX $CXXFLAGS list01more.cc -o $OUT/list01more

$CXX $CXXFLAGS list02.cc -o $OUT/list02
$CXX $CXXFLAGS list02const.cc -o $OUT/list02const

$CXX $CXXFLAGS list03.cc -o $OUT/list03
