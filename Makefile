# ROMs
Target_ROM := RLFE.gba
Source_ROM := FE8_clean.gba

# Directories
Tools := $(realpath .)/Tools
Make := $(realpath .)/Make
CHAX := $(realpath .)/CHAX
Spritans := $(realpath .)/Spritans
Maps := $(realpath .)/Maps
Events := $(realpath .)/Events
Tables := $(realpath .)/Tables

# EA
EA := $(Tools)/EventAssembler
Core := $(EA)/ColorzCore
EA_Input := Main.event

# Include
include $(Make)/Wizardry.mak
include $(Make)/Spritans.mak
include $(Make)/Maps.mak
include $(Make)/Events.mak
include $(Make)/Tables.mak

# make hack
hack: $(Target_ROM)

$(Target_ROM): $(TableDefinitions) $(TableInit) $(TableInstaller) $(EventInstaller) $(MapInstaller) $(MapSpriteInstaller) $(CHAXInstaller) $(EA_Input)
	@echo Building $(Target_ROM)...
	@cp $(Source_ROM) $(Target_ROM)
	@$(Core) A FE8 -output:$(Target_ROM) -input:$(EA_Input) --nocash-sym --build-times
