import argparse
import sys
import os
import textwrap
from PIL import Image

parser = argparse.ArgumentParser()

parser.add_argument('--input')
parser.add_argument('--output')
parser.add_argument('--relative-path')
parser.add_argument('--start-index')

args = parser.parse_args()
args.input = args.input.split(' ')

class StandInfile:
	def __init__(self, path, job, index, size):
		self.path = path
		self.job = job
		self.index = index
		self.size = size

class MoveInfile:
	def __init__(self, path, job, ap):
		self.path = path
		self.job = job
		self.ap = ap

standinfiles = []
moveinfiles = []

args.start_index = int(args.start_index, 0)

if (args.relative_path is not None):
	for infile in args.input:
		if infile != '':
			common = os.path.commonprefix([infile, args.relative_path])
			path = os.path.relpath(infile, common)
			msfile = os.path.basename(infile).split('.')[0].split('_')
			job = msfile[0]
			if 'stand' in infile.lower():
				index = 0
				if len(msfile) == 3:
					index = msfile[2]
				else:
					index = args.start_index
					args.start_index += 1
				imagefile = infile.replace('dmp', 'png')
				image = Image.open(imagefile)
				w, h = image.size
				image.close()
				size = 0
				if (w*h) == (16*32*3):
					size = 1
				elif (w*h) == (32*32*3):
					size = 2
				standinfiles.append(StandInfile(path, job, index, size))
			elif 'move' in infile.lower():
				if len(msfile) == 3:
					ap = msfile[2]
				else:
					ap = 'DefaultAP'
				job = msfile[0]
				moveinfiles.append(MoveInfile(path, job, ap))

with open(args.output, 'w') as file:
	file.write(textwrap.dedent(f'''\
		// File output by MapSpritesToInstaller
		// Do not edit!

		#define DefaultAP 0x81CAA20

		#define StandMapSpriteEntry(index, pattern, size, data) "PUSH; ORG StandMapSpriteTable + (index*8); SHORT pattern size; POIN data; POP"
		#define MoveMapSpriteEntry(index, ap, data) "PUSH; ORG MoveMapSpriteTable + ((index-1)*8); POIN data ap; POP"

	'''))

	for infile in standinfiles:
		file.write(textwrap.dedent(f'''\
			#define {infile.job}StandMapSprite {infile.index}
			ALIGN 4
			{infile.job}StandMapSpriteData:
			#incbin "{infile.path}"
			StandMapSpriteEntry({infile.job}StandMapSprite, 0, {infile.size}, {infile.job}StandMapSpriteData)

		'''))

	for infile in moveinfiles:
		file.write(textwrap.dedent(f'''\
			ALIGN 4
			{infile.job}MoveMapSpriteData:
			#incbin "{infile.path}"
			StandMapSpriteEntry({infile.job}, {infile.ap}, {infile.job}MoveMapSpriteData)

		'''))
	file.close()
