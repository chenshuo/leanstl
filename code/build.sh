#!/bin/sh

set -x

CXX=g++
CXXFLAGS="-std=c++11 -Wall -g"
OUT=bin

mkdir -p $OUT

$CXX $CXXFLAGS -fno-access-control list00.cc -o $OUT/list00
$CXX $CXXFLAGS -fno-access-control list00.cc -m32 -o $OUT/list00m32
$CXX $CXXFLAGS -fsanitize=address list01err0.cc -o $OUT/list01err0asan
#$CXX $CXXFLAGS -fsanitize=memory list03err.cc -o $OUT/list03errmsan
#$CXX $CXXFLAGS -fsanitize=memory list04err.cc -o $OUT/list04errmsan

##### AUTO UPDATE

$CXX $CXXFLAGS list01.cc -o $OUT/list01
$CXX $CXXFLAGS list01alt.cc -o $OUT/list01alt
$CXX $CXXFLAGS list01err0.cc -o $OUT/list01err0
$CXX $CXXFLAGS list01err0hack.cc -o $OUT/list01err0hack
$CXX $CXXFLAGS list01err1.cc -o $OUT/list01err1
$CXX $CXXFLAGS list02.cc -o $OUT/list02
$CXX $CXXFLAGS list02const.cc -o $OUT/list02const
$CXX $CXXFLAGS list03.cc -o $OUT/list03
$CXX $CXXFLAGS list03err.cc -o $OUT/list03err
$CXX $CXXFLAGS list04.cc -o $OUT/list04
$CXX $CXXFLAGS list04err.cc -o $OUT/list04err
$CXX $CXXFLAGS list04more.cc -o $OUT/list04more
