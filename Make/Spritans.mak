# Directories
MapSprites = $(Spritans)/MapSprites
MapSpriteInstaller = $(Spritans)/MapSprites/Installer.event

# Python
Png2Dmp = $(Tools)/Python/Png2Dmp.py
MapSpritesToInstaller = $(Tools)/Python/MapSpritesToInstaller.py

# General PNG to DMP rule
%.dmp: %.png
	@python3 $(Png2Dmp) --input '$<' --output '$@'

# Map Sprite rule
$(MapSprites)/dmp/%.dmp: $(MapSprites)/png/%.png
	@mkdir -p $(dir $@)
	@python3 $(Png2Dmp) --input '$<' --output '$@' --lz77

MAPSPRITE_PNG := $(shell find $(MapSprites)/png -name '*.png')
MAPSPRITE_DMP := $(patsubst $(MapSprites)/png/%.png, $(MapSprites)/dmp/%.dmp, $(MAPSPRITE_PNG))

$(MapSpriteInstaller): $(MAPSPRITE_DMP)
	@python3 $(MapSpritesToInstaller) --output '$(MapSpriteInstaller)' --input '$(MAPSPRITE_DMP)' --relative-path '$(MapSprites)' --start-index '0x6B'
