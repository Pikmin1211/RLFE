import argparse
import sys
import os
import textwrap

parser = argparse.ArgumentParser()

parser.add_argument('--input')
parser.add_argument('--output')
parser.add_argument('--relative-path')

args = parser.parse_args()
args.input = args.input.split(' ')

if (args.relative_path is not None):
	infiles = []
	for infile in args.input:
		if infile != '':
			common = os.path.commonprefix([infile, args.relative_path])
			infiles.append(os.path.relpath(infile, common))

with open(args.output, 'w') as file:
	file.write(textwrap.dedent(f'''\
		// File output by DmpFilesToInstaller
		// Do not edit!
	'''))

	for infile in infiles:
		file.write(textwrap.dedent(f'''\
			ALIGN 4
			{os.path.basename(infile).split('.')[0]}:
			#incbin "{infile}"

		'''))
	file.close()
