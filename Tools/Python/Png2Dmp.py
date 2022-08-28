import argparse
import sys
import os
import ImageGBA as igba
import lz77

parser = argparse.ArgumentParser()

parser.add_argument('--input')
parser.add_argument('--output')
parser.add_argument('--palette-output')
parser.add_argument('--palette-size')
parser.add_argument('--lz77', action = 'store_true')
parser.add_argument('--palette', action = 'store_true')
parser.add_argument('--lz77-palette', action = 'store_true')

args = parser.parse_args()

if (os.path.splitext(args.input)[1] != '.png'):
	raise ValueError('Input must be a png file!')

if (args.output is None):
	args.output = args.input.replace('.png', '.dmp')

if (args.palette_output is None and args.palette):
	args.palette_output = args.input.replace('.png', '_pal.dmp')
elif (args.palette_output is not None and not args.palette):
	args.palette = True

if (args.palette_size is None):
	args.palette_size = 16

if (not args.lz77):
	with open(args.output, 'wb') as outfile:
		outfile.write(igba.get_bytes(args.input))
		outfile.close()
else:
	with open(args.output, 'wb') as outfile:
		outfile.write(lz77.compress(igba.get_bytes(args.input)))
		outfile.close()

if (args.palette and not args.lz77_palette):
	with open(args.palette_output, 'wb') as outfile:
		outfile.write(igba.get_palette(args.input, args.palette_size))
		outfile.close()
elif (args.palette):
	with open(args.palette_output, 'wb') as outfile:
		outfile.write(lz77.compress(igba.get_palette(args.input, args.palette_size)))
		outfile.close()
