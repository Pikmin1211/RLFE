# Directories
src := $(CHAX)/src
src_Include := $(CHAX)/include
CLib := $(CHAX)/FE-CLib
clib_Include := $(CLib)/include
CHAXInstaller := $(CHAX)/Installer.event

# Lyn
LYN := $(Tools)/Lyn/lyn
LYN_REFERENCE := $(CHAX)/Definitions.o

# DevKit
include $(DEVKITARM)/base_tools

# Include Flags
INCLUDE_DIRS := $(clib_Include) $(src_Include) $(Tables)
INCFLAGS := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

# Compilation Flags
ARCH := -mcpu=arm7tdmi -mthumb -mthumb-interwork
CFLAGS := $(ARCH) $(INCFLAGS) -Wall -O2 -mtune=arm7tdmi -ffreestanding -fomit-frame-pointer -mlong-calls
ASFLAGS := $(ARCH) $(INCFLAGS)

# Python
FilesToInstaller = $(Tools)/Python/FilesToInstaller.py

.PRECIOUS: %.o;

# OBJ to event rule
$(src)/event/%.lyn.event: $(src)/obj/%.o $(LYN_REFERENCE)
	@mkdir -p $(dir $@)
	@$(LYN) $< $(LYN_REFERENCE) > $@

# OBJ to DMP rule
$(src)/dmp/%.dmp: $(src)/obj/%.o
	@mkdir -p $(dir $@)
	@$(OBJCOPY) -S $< -O binary $@

# ASM to OBJ rule
$(src)/obj/%.o: $(src)/asm/%.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -I $(dir $<) $< -o $@

# ASM to OBJ rule
%.o: %.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -I $(dir $<) $< -o $@

# C to ASM rule
$(src)/obj/%.o: $(src)/c/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# C to ASM rule
$(src)/asm/%.asm: $(src)/c/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -S $< -o $@ -fverbose-asm

# ASM/C and generated files
CFILES := $(shell find $(src)/c -name '*.c')
LYNFILES := $(patsubst $(src)/c/%.c, $(src)/event/%.lyn.event, $(CFILES))

$(CHAXInstaller): $(FilesToInstaller) $(LYNFILES)
	@python3 $(FilesToInstaller) --output '$(CHAXInstaller)' --input '$(LYNFILES)' --relative-path '$(CHAX)'