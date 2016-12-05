#!/bin/sh

set -x

mkdir -p diff

diff -u /dev/null list00.cc             > diff/list00.cc.diff

##### AUTO UPDATE

