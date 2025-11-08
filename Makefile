BUILD_DIR := build
LIB_DIRS := lib
OUTPUT_NAME := mm_recomp_rando
MOD_TOML := mod.toml

MOD_TOOL := ./RecompModTool
OFFLINE_RECOMP := ./OfflineModRecomp
TARGET  := $(BUILD_DIR)/mod.elf
MANIFEST := $(wildcard $(OUTPUT_NAME)/$(OUTPUT_NAME)*.nrm)
OUTPUT_NAME_W_VER := $(notdir $(MANIFEST:.nrm=))

LIBFILES := $(foreach ld, $(LIB_DIRS), $(wildcard $(ld)/*.dll))

LDSCRIPT := mod.ld
CFLAGS   := -target mips -mips2 -mabi=32 -O2 -G0 -mno-abicalls -mno-odd-spreg -mno-check-zero-division \
			-fomit-frame-pointer -ffast-math -fno-unsafe-math-optimizations -fno-builtin-memset \
			-Wall -Wextra -Wno-incompatible-library-redeclaration -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable \
			-Wno-missing-braces -Wno-unsupported-floating-point-opt -Werror=section
CPPFLAGS := -nostdinc -D_LANGUAGE_C -DMIPS -I include -I include/dummy_headers -I mm-decomp/include -I mm-decomp/src -I mm-decomp/assets
LDFLAGS  := -nostdlib -T $(LDSCRIPT) -Map $(BUILD_DIR)/mod.map --unresolved-symbols=ignore-all --emit-relocs -e 0 --no-nmagic

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
getdirs = $(sort $(dir $(1)))

C_SRCS := $(call rwildcard,src,*.c)
C_OBJS := $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.o))
C_DEPS := $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.d))

BUILD_DIRS := $(call getdirs,$(C_OBJS))

$(OUTPUT_NAME)/mod_binary.bin: $(TARGET) $(MOD_TOML) rando_syms.toml $(LIBFILES) | $(OUTPUT_NAME)
	$(MOD_TOOL) $(MOD_TOML) $(OUTPUT_NAME)

ifeq ($(OS),Windows_NT)

CC      = ./bin/clang
LD      = ./bin/ld.lld

ifeq ($(MSYSTEM),MINGW64)

define make_folder
	bash -c "mkdir -p $@"
endef

clean:
	bash -c "rm -rf $(BUILD_DIR) $(OUTPUT_NAME)"

else

define make_folder
	mkdir $(subst /,\,$(1))
endef

clean:
	del /s /q /f /a $(BUILD_DIR)
	del /s /q /f /a $(OUTPUT_NAME)

endif

$(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER).dll: build/mod_recompiled.c
ifeq ($(MANIFEST),)
	@$(MAKE) offline --no-print-directory
else
	@$(MAKE) endlib --no-print-directory
endif

endlib:
	clang-cl build/mod_recompiled.c -Wno-macro-redefined -fuse-ld=lld -Z7 /Ioffline_build -MD -O2 /link /DLL /OUT:$(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER).dll

offline: $(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER).dll

else

define make_folder
	mkdir -p $(1)
endef

ifeq ($(shell uname),Darwin)
	# Allow the user to specify the compiler and linker on macOS
	# as Apple Clang does not support MIPS architecture
	CC         ?= clang
	LD         ?= ld.lld

	LIB_SUFFIX := .dylib
	ARCH_FLAGS := -arch x86_64 -arch arm64
else
	CC         := clang
	LD         := ld.lld

	LIB_SUFFIX := .so
	ARCH_FLAGS :=
endif

$(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER)$(LIB_SUFFIX): build/mod_recompiled.c
ifeq ($(MANIFEST),)
	@$(MAKE) offline --no-print-directory
else
	@$(MAKE) endlib --no-print-directory
endif

endlib:
	clang build/mod_recompiled.c -Wno-macro-redefined -shared -fvisibility=hidden -fPIC -O2 -Ioffline_build $(ARCH_FLAGS) -o $(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER)$(LIB_SUFFIX)

offline: $(OUTPUT_NAME)/$(OUTPUT_NAME_W_VER)$(LIB_SUFFIX)

clean:
	bash -c "rm -rf $(BUILD_DIR) $(OUTPUT_NAME)"

endif


build/mod_recompiled.c: $(OUTPUT_NAME)/mod_binary.bin
	$(OFFLINE_RECOMP) $(OUTPUT_NAME)/mod_syms.bin $(OUTPUT_NAME)/mod_binary.bin Zelda64RecompSyms/mm.us.rev1.syms.toml $@

$(OUTPUT_NAME):
	$(call make_folder, $@)

$(TARGET): $(C_OBJS) $(LDSCRIPT) | $(BUILD_DIR)
	$(LD) $(C_OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR) $(BUILD_DIRS):
	$(call make_folder, $@)

$(C_OBJS): $(BUILD_DIR)/%.o : %.c | $(BUILD_DIRS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

-include $(C_DEPS)

.PHONY: endlib offline clean
