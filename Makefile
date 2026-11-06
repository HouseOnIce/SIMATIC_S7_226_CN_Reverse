SDK := ../APM32E10x_SDK/Libraries
DEVICE := $(SDK)/Device/Geehy/APM32E10x
CMSIS_CORE := $(SDK)/CMSIS/CM3/CoreSupport
CMSIS_INC := $(SDK)/CMSIS/CM3/DeviceSupport
DRIVER := $(SDK)/APM32E10x_StdPeriphDriver
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
CFLAGS := -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -std=c11 -O0 -g3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fsigned-char -Wall -Wextra -DAPM32E10x_HD -DUSE_STDPERIPH_DRIVER -Iinclude -I$(DEVICE)/Include -I$(CMSIS_CORE) -I$(DRIVER)/inc
LDFLAGS := -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -nostartfiles -Wl,--gc-sections -Tlinker/apm32e103vet6.ld
APP_SRC := $(filter-out src/startup_apm32e103.c,$(wildcard src/*.c)) platform/system_apm32e10x.c
OBJ := $(patsubst %.c,build/%.o,$(APP_SRC)) build/platform/startup_apm32e10x_hd.o

.PHONY: all clean sources-check
all: build/s7_226_reconstructed.elf build/s7_226_reconstructed.bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/platform/startup_apm32e10x_hd.o: $(DEVICE)/Source/gcc/startup_apm32e10x_hd.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

build/s7_226_reconstructed.elf: $(OBJ) linker/apm32e103vet6.ld
	$(CC) $(LDFLAGS) $(OBJ) -o $@

build/s7_226_reconstructed.bin: build/s7_226_reconstructed.elf
	$(OBJCOPY) -O binary $< $@

sources-check:
	@for f in $(APP_SRC); do $(CC) $(CFLAGS) -fsyntax-only $$f || exit 1; done

clean:
	rm -rf build
