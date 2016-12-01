#!/usr/bin/python

# Preprocess markdown

import re, sys

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

if len(sys.argv) == 1:
	# TODO watch for files changes
	pass
else:
	for f in sys.argv[1:]:
		process(f)
