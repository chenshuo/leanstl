#!/bin/bash

set -x

CXX=g++
CXXFLAGS="-std=c++11 -Wall -g"
OUT=bin

mkdir -p $OUT

$CXX $CXXFLAGS list00.cc -o $OUT/list00
$CXX $CXXFLAGS list00.cc -m32 -o $OUT/list00m32

$CXX $CXXFLAGS list01.cc -o $OUT/list01
$CXX $CXXFLAGS list01err0.cc -o $OUT/list01err0
$CXX $CXXFLAGS list01err1.cc -o $OUT/list01err1
$CXX $CXXFLAGS list01a.cc -o $OUT/list01a
