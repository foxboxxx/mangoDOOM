################################################################
#
# $Id:$
#
# $Log:$
#

ifeq ($(V),1)
	VB=''
else
	VB=@
endif

CC=riscv64-unknown-elf-gcc
CROSS_COMPILE = riscv64-unknown-elf-

# PRODUCTS = $(SRC_PROGRAMS:.c=.bin)
# DEFAULT_NAME = $(firstword $(SRC_PROGRAMS:.c=))

# Flags for compile and link
ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include $$warn $$freestanding -fno-omit-frame-pointer -fstack-protector-strong -fstrict-volatile-bitfields -nostdlib
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc
OBJDUMP_FLAGS = -d --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps=color
GDB_FLAGS= -q --command=$$CS107E/other/gdbsim.commands

# Access .c and .s source files from SHARED directory using vpath
vpath %.c $$CS107E/../extras/src
vpath %.s $$CS107E/../extras/src
CFLAGS += -I$$CS107E/../extras/src
vpath %.c ./pseudo_include
CFLAGS += -I./pseudo_include

GDB_FLAGS= -q --command=$$CS107E/other/gdbsim.commands


# subdirectory for objects
OBJDIR = build
OUTPUT = doomgeneric

SRC_C = doomgeneric_mango.c strings_extension.c malloc_extension.c ctype.c dummy.c am_map.c doomdef.c \
        doomstat.c dstrings.c d_event.c d_items.c d_iwad.c d_loop.c d_main.c \
		d_mode.c d_net.c f_finale.c f_wipe.c g_game.c hu_lib.c hu_stuff.c \
		info.c i_cdmus.c i_endoom.c i_joystick.c i_scale.c i_sound.c i_system.c \
		i_timer.c memio.c m_argv.c m_bbox.c m_cheat.c m_config.c m_controls.c \
		m_fixed.c m_menu.c m_misc.c m_random.c p_ceilng.c p_doors.c p_enemy.c \
		p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c \
		p_pspr.c p_saveg.c p_setup.c p_sight.c p_spec.c p_switch.c p_telept.c \
		p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c r_segs.c \
		r_sky.c r_things.c sha1.c sounds.c statdump.c st_lib.c st_stuff.c s_sound.c\
		tables.c v_video.c wi_stuff.c w_checksum.c w_file.c w_main.c w_wad.c \
		z_zone.c w_file_stdc.c i_input.c i_video.c doomgeneric.c doom_wad.c 
		
OBJS += $(addprefix $(OBJDIR)/, $(SRC_C:.c=.o))

all: $(OUTPUT).bin

$(OUTPUT).debug: $(OUTPUT.elf)
	riscv64-unknown-elf-gdb $(GDB_FLAGS) $<

$(OUTPUT).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OUTPUT).bin: $(OUTPUT).elf
	$(CROSS_COMPILE)objcopy $< -O binary $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

run: $(OUTPUT).bin
	mango-run $<

debug: $(OUTPUT).debug

clean:
	rm -rf $(OBJDIR) $(OUTPUT).elf $(OUTPUT).bin

.PHONY: all run clean debug
# .PRECIOUS: %.elf %.o


# all:	 $(OUTPUT)

# clean:
# 	rm -rf $(OBJDIR)
# 	rm -f $(OUTPUT)
# 	rm -f $(OUTPUT).gdb
# 	rm -f $(OUTPUT).map

# $(OUTPUT):	$(OBJS)
# 	@echo [Linking $@]
# 	$(VB)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
# 	-o $(OUTPUT) $(LDLIBS) -Wl,-Map,$(OUTPUT).map
# 	@echo [Size]
# 	-$(CROSS_COMPILE)size $(OUTPUT)

# $(OBJS): | $(OBJDIR)

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# $(OBJDIR)/%.o:	%.c
# 	@echo [Compiling $<]
# 	$(VB)$(CC) $(CFLAGS) -c $< -o $@

# print:
# 	@echo OBJS: $(OBJS)