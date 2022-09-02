import json
import os
import romtools as rt
import tablejson

rom = rt.rom(f'{os.getcwd()}\\FE8_clean.gba')
tabledir = f'{os.getcwd()}\\Character'
tablejson.dumpfromlist(tabledir, rom)
