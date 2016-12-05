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


IB = re.compile('@ib ([^ ]+) "(.*)"')
def include_block(command, out):
	m = IB.match(command)
	src = m.group(1)
	block = m.group(2)
	print '  include', src
	out.write('```cpp\n')
	is_in = False
	level = 0
	with open('code/'+src) as f:
		for line in f:
			if line.startswith(block):
				is_in = True
			if is_in:
				out.write(line)
				if line.strip().startswith('{'):
					level += 1
				if line.strip().startswith('}'):
					level -= 1
					if level == 0:
						break

	out.write('```\n')


def process(f):
	fo = 'docs/' + f[:-3]
	print 'processing', f, '->', fo
	with open(f) as md, open(fo, 'w') as out:
		for line in md:
			if line.startswith('@i '):
				include(line, out)
			elif line.startswith('@ib '):
				include_block(line, out)
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
