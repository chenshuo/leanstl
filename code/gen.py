#!/usr/bin/python

GRAPH = """
list00 -> list01 -> list02 -> list02const -> list03 -> list04 -> list04more

list01 -> list01alt
list01 -> list01err0
list01err0 -> list01err0hack
list01 -> list01err1

list03 -> list03err
list04 -> list04err
"""

all_targets = set()

for line in GRAPH.splitlines():
    targets = map(lambda x: x.strip(), line.split('->'))
    if len(targets) < 2:
        continue
    # print targets
    all_targets.update(targets)
    for i in range(len(targets)-1):
        orig = targets[i]+'.cc'
        dest = targets[i+1]+'.cc'
        print 'diff -u %(orig)s %(dest)s --label %(orig)s --label %(dest)s > diff/%(dest)s.diff' % {'orig': orig, 'dest': dest }

with open('build.sh', 'r+') as f:
    while True:
        line = f.readline()
        if 'AUTO UPDATE' in line:
            f.write('\n')
            break
    for target in sorted(all_targets):
        f.write('$CXX $CXXFLAGS %(target)s.cc -o $OUT/%(target)s\n' % {'target': target})
    f.truncate()

