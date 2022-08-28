# ROMs
Target_ROM := FE8_hack.gba
Source_ROM := FE8_clean.gba

# Directories
Tools := $(realpath .)/Tools
Make := $(realpath .)/Make
CHAX := $(realpath .)/CHAX
Spritans := $(realpath .)/Spritans
Maps := $(realpath .)/Maps
Events := $(realpath .)/Events

# EA
EA := $(Tools)/EventAssembler
Core := $(EA)/ColorzCore
EA_Input := Main.event

# Include
include $(Make)/Wizardry.mak
include $(Make)/Spritans.mak
include $(Make)/Maps.mak
include $(Make)/Events.mak

# make hack
hack: $(Target_ROM)

$(Target_ROM): $(EventInstaller) $(MapInstaller) $(MapSpriteInstaller) $(CHAXInstaller) $(EA_Input)
	@echo Building $(Target_ROM)...
	@cp $(Source_ROM) $(Target_ROM)
	@$(Core) A FE8 -output:$(Target_ROM) -input:$(EA_Input) --nocash-sym --build-times
