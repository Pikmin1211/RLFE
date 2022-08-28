# Directories
EventInstaller = $(Events)/Installer.event

# Python
FilesToInstaller = $(Tools)/Python/FilesToInstaller.py

EVENTFILES := $(shell find $(Events)/Event -name '*.event')

$(EventInstaller): $(EVENTFILES)
	@python3 $(FilesToInstaller) --output '$(EventInstaller)' --input '$(EVENTFILES)' --relative-path '$(Events)'
