import argparse
import sys
import os
import textwrap
import tablejson

parser = argparse.ArgumentParser()

parser.add_argument('--input')
parser.add_argument('--output')

args = parser.parse_args()
tablejson.eventfromjson(args.input, args.output)

