import argparse
import sys
import os
import textwrap
import tablejson

parser = argparse.ArgumentParser()

parser.add_argument('--input')
parser.add_argument('--output')

args = parser.parse_args()
args.input = args.input.split(' ')

infiles = []
for infile in args.input:
	if infile != '':
		infiles.append(infile)

tablejson.makedefinitionfile(infiles, args.output)