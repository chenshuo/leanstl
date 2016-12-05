#!/usr/bin/python

# Preprocess markdown

import re, sys
import pyinotify, livereload
import tornado.ioloop

def include(command, out):
	(_, src, ranges) = command.split()
	print '  include', src
	out.write('```cpp\n')
	with open('code/'+src) as f:
		lines = f.readlines()
		for r in ranges.split(','):
			(start, end) = r.split('-')
			out.writelines(lines[int(start)-1:int(end)])
	out.write('```\n')

def process(f):
	fo = 'docs/' + f[:-3]
	print 'processing', f, '->', fo
	with open(f) as md, open(fo, 'w') as out:
		for line in md:
			if line.startswith('@i '):
				include(line, out)
			else:
				out.write(line)

def build():
	process('list.md.in')

if len(sys.argv) == 1:
	watcher = livereload.watcher.get_watcher_class()()
	watcher.watch("*.in")
	watcher.watch("code/*.cc")
	watcher.start(build)
	tornado.ioloop.IOLoop.instance().start()
else:
	for f in sys.argv[1:]:
		process(f)
