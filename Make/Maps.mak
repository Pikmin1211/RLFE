# Directories
MapInstaller = $(Maps)/Installer.event

# Python
tmx2ea = $(Tools)/Python/tmx2ea.py
FilesToInstaller = $(Tools)/Python/FilesToInstaller.py

# tmx to event and dmp rule
$(Maps)/event/%.event $(Maps)/dmp/%.dmp: $(Maps)/tmx/%.tmx
	@mkdir -p $(dir $(Maps)/event/$*.event)
	@mkdir -p $(dir $(Maps)/dmp/$*.dmp)
	@python3 $(tmx2ea) --input '$<' --output '$(Maps)/event/$*.event' --output-dmp '$(Maps)/dmp/$*.dmp'
	
MAP_TMX := $(shell find $(Maps)/tmx -name '*.tmx')
MAP_EVENT := $(patsubst $(Maps)/tmx/%.tmx, $(Maps)/event/%.event, $(MAP_TMX))
MAP_DMP := $(patsubst $(Maps)/tmx/%.tmx, $(Maps)/dmp/%.dmp, $(MAP_TMX))

$(MapInstaller): $(MAP_EVENT) $(MAP_DMP)
	@python3 $(FilesToInstaller) --input $(MAP_EVENT) --output $(MapInstaller) --relative-path $(Maps)
