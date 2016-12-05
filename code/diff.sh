#!/bin/sh

set -x

mkdir -p diff

diff -u /dev/null list00.cc             > diff/list00.cc.diff
diff -u list00.cc list01.cc             > diff/list01.cc.diff

diff -u list01.cc list01alt.cc          > diff/list01alt.cc.diff
diff -u list01.cc list01err0.cc         > diff/list01err0.cc.diff
diff -u list01err0.cc list01err0hack.cc > diff/list01err0hack.cc.diff
diff -u list01.cc list01err1.cc         > diff/list01err1.cc.diff

diff -u list01.cc list02.cc             > diff/list02.cc.diff

diff -u list02.cc list02const.cc        > diff/list02const.cc.diff

diff -u list02const.cc list03.cc        > diff/list03.cc.diff

diff -u list03.cc list03ctor.cc         > diff/list03ctor.cc.diff
