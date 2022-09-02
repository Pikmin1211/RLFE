# Directories
TableInit = $(Tables)/Init.event
TableInstaller = $(Tables)/Installer.event
TableDefinitions = $(Tables)/Definitions.event

# Python
json2event = $(Tools)/Python/TableJSON/json2event.py
djson2event = $(Tools)/Python/TableJSON/djson2event.py
json2def = $(Tools)/Python/TableJSON/json2def.py
FilesToInstaller = $(Tools)/Python/FilesToInstaller.py

# json to event rule
$(Tables)/event/%.event: $(Tables)/json/%.json
	@mkdir -p $(dir $(Tables)/event/$*.event)
	@python3 $(json2event) --input '$<' --output '$(Tables)/event/$*.event'

TABLE_JSON := $(shell find $(Tables)/json -name '*.json')
TABLE_DJSON := $(shell find $(Tables)/json -name '*.djson')
TABLE_DMP := $(shell find $(Tables)/json -name '*.dmp')
TABLE_EVENT := $(patsubst $(Tables)/json/%.json, $(Tables)/event/%.event, $(TABLE_JSON))

$(TableInit): $(TABLE_DJSON) $(TABLE_DMP)
	@python3 $(djson2event) --input '$(TABLE_DJSON)' --output $(TableInit) --relative-path $(Tables)

$(TableInstaller): $(TABLE_EVENT)
	@python3 $(FilesToInstaller) --input '$(TABLE_EVENT)' --output $(TableInstaller) --relative-path $(Tables)

$(TableDefinitions): $(TABLE_JSON)
	@python3 $(json2def) --input '$(TABLE_JSON)' --output $(TableDefinitions)