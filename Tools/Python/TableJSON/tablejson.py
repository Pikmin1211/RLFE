import json
import os
import romtools as rt
import re
import textwrap

def getdatasize(datatype: str):
	return re.sub("[^0-9]", "", datatype)

def hexstr(val: int, nbits: int):
    return hex((val + (1 << nbits)) % (1 << nbits))

def dumpjsonfromrom(dir: str, rom: rt.rom, index: int, name: str):
	defJson = f'{dir}/_Def.djson'
	dumpJson = f'{dir}/Json/{name}.json'

	with open(defJson, 'r') as file:
		defDump = json.load(file)
		file.close()

	dumpData = {}
	dumpData['header'] = {}
	dumpData['fields'] = {}

	tableoffset = int(defDump['header']['address'],0)
	tableentrysize = int(defDump['header']['entry_size'],0)
	if (defDump['header']['is_zero_indiced'].lower() == 'true'):
		rom.setoffset(tableoffset + (tableentrysize * index))
	elif (defDump['header']['is_zero_indiced'].lower() == 'false'):
		rom.setoffset(tableoffset + (tableentrysize * (index-1)))

	for field in defDump['fields']:
		datatype = defDump['fields'][field]
		datasize = int(getdatasize(datatype),0)
		datasigned = False
		if 's' in datatype:
			datasigned = True
		value = rom.readbytes(datasize/8, datasigned)
		if 'h' in datatype:
			dumpData['fields'][field] = hexstr(value, datasize)
		else:
			dumpData['fields'][field] = str(value)

	dumpData['header']['name'] = name
	dumpData['header']['index'] = dumpData['fields'][defDump['header']['index_entry']]

	with open(dumpJson, 'w+') as file:
		json.dump(dumpData, file, indent = 4)
		file.close()

def dumpfromlist(dir: str, rom: rt.rom):
	index = 0
	defJson = f'{dir}/_Def.djson'
	
	with open(defJson, 'r') as file:
		defDump = json.load(file)
		file.close()

	if (defDump['header']['is_zero_indiced'].lower() == 'false'):
		index += 1

	with open(f'{dir}/_List.txt', 'r') as entrylist:
		for entry in entrylist.readlines():
			dumpjsonfromrom(dir, rom, index, entry.strip())
			index += 1
		file.close()

def eventfromjson(infile: str, outfile: str):
	defdir = os.path.dirname(infile)
	while(defdir != os.path.dirname(defdir)):
		deffile = defdir + '/_Def.djson'
		if (os.path.isfile(deffile)):
			break
		defdir = os.path.dirname(defdir)

	with open(infile, 'r') as file:
		data = json.load(file)
		file.close()

	with open(deffile, 'r') as file:
		defDump = json.load(file)
		file.close()

	values = ''

	for field in defDump['fields']:
		datatype = defDump['fields'][field]
		datasize = int(getdatasize(datatype),0)
		datasigned = False
		earaw = ''
		if 'p' in datatype:
			earaw = 'POIN'
		else:
			if datasize == 32:
				earaw = 'WORD'
			elif datasize == 16:
				earaw = 'SHORT'
			elif datasize == 8:
				earaw = 'BYTE'
		values += f"{earaw} ({data['fields'][field]})\n"

	index = int(data["header"]["index"], 0)
	if (defDump['header']['is_zero_indiced'].lower() == 'false'):
		index -= 1

	with open(outfile, 'w') as file:
		file.write(textwrap.dedent(f'''\
			// File output by TableJSON
			// Do not edit!

			PUSH
			ORG {defDump["header"]["name"]} + ({defDump["header"]["entry_size"]} * {index})
		'''))

		file.write(values)
		file.write('POP')
		file.close()

def makedefinitionfile(infiles: list, outfile: str):
	with open(outfile, 'w') as wfile:
		wfile.write(textwrap.dedent(f'''\
			// File output by TableJSON
			// Do not edit!

		'''))
		for infile in infiles:
			with open(infile, 'r') as file:
				dumpData = json.load(file)
				file.close()
			wfile.write(f'#define {dumpData["header"]["name"]} {dumpData["header"]["index"]}\n')
		wfile.close

def maketablefile(infiles: list, outfile: str, relative_path: str):
	with open (outfile, 'w') as wfile:
		wfile.write(textwrap.dedent(f'''\
			// File output by TableJSON
			// Do not edit!

		'''))
		for infile in infiles:
			with open(infile, 'r') as file:
				defDump = json.load(file)
				file.close()
			tabledmp = os.path.dirname(infile) + '/Table.dmp'
			if (os.path.isfile(tabledmp)):
				common = os.path.commonprefix([tabledmp, relative_path])
				tabledmp = os.path.relpath(tabledmp, common)
				wfile.write(textwrap.dedent(f'''\
					ALIGN 4
					{defDump["header"]["name"]}:
					#incbin "{tabledmp}"
				'''))
			else:
				wfile.write(textwrap.dedent(f'''\
					ALIGN 4
					{defDump["header"]["name"]}:
					ORG currentOffset + ({defDump["header"]["num_entries"]} * {defDump["header"]["entry_size"]})
				'''))
			wfile.write('\nPUSH\n')
			for address in defDump['header']['pointers']:
				wfile.write(textwrap.dedent(f'''\
					ORG {address}
					POIN {defDump["header"]["name"]}
				'''))
			for specialaddress in defDump['header']['special_pointers']:
				wfile.write(textwrap.dedent(f'''\
					ORG {specialaddress[0]}
					POIN {defDump["header"]["name"]} + ({defDump["header"]["entry_size"]} * {specialaddress[1]})
				'''))
			wfile.write('POP\n')