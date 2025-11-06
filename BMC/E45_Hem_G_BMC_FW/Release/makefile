RM := rm -rf

BUILD_ARTIFACT_NAME := E45_BMC_FW_LL
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT := $(BUILD_ARTIFACT_NAME).elf

# Source and object files
SOURCES := $(shell find ../Core ../Drivers ../Init ../sdk -name '*.c' ! -name '*.su')
ASM_SOURCES := $(shell find ../Core/Startup -name '*.s' ! -name '*.su')

C_OBJECTS := $(SOURCES:.c=.o)
ASM_OBJECTS := $(ASM_SOURCES:.s=.o)
ALL_OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)  # Start-up files first

# Compiler flags (match CubeIDE)
CFLAGS = \
    -mcpu=cortex-m0plus \
    -mthumb \
    -mfloat-abi=soft \
    -Og \
    -Wall \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin -fno-common \
    -std=gnu11 \
    -DDEBUG \
    -DUSE_FULL_LL_DRIVER \
    -DSTM32C031xx \
    -DHSE_VALUE=8000000 \
    -DHSI_VALUE=48000000 \
    -DLSE_VALUE=32768 \
    -DLSI_VALUE=32000 \
    -DHSE_STARTUP_TIMEOUT=100 \
    -DLSE_STARTUP_TIMEOUT=5000 \
    -DEXTERNAL_CLOCK_VALUE=12288000 \
    -DVDD_VALUE=3300 \
    -DPREFETCH_ENABLE=0 \
    -DINSTRUCTION_CACHE_ENABLE=1 \
    -DDATA_CACHE_ENABLE=1 \
    -I../Core/Inc \
    -I../Drivers/STM32C0xx_HAL_Driver/Inc \
    -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy \
    -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include \
    -I../Drivers/CMSIS/Include \
    -I../sdk/Inc \
    -I../Init/Inc \
    -I../Core/Drivers/Button/Inc \
    -I../Core/Drivers/Leds/Inc

# Rules
%.o: %.c
	@echo "Compiling: $<"
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	@echo "Assembling: $<"
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -x assembler-with-cpp -c $< -o $@

# Linker flags (match CubeIDE)
LDFLAGS = \
	-mcpu=cortex-m0plus \
	-T"../STM32C031G4UX_FLASH.ld" \
	--specs=nosys.specs \
	-Wl,-Map=$(BUILD_ARTIFACT_NAME).map \
	-Wl,--gc-sections \
	--specs=nano.specs \
	-mfloat-abi=soft -mthumb \
	-Wl,--start-group -lc -lm -Wl,--end-group

# Targets
all: $(BUILD_ARTIFACT)

$(BUILD_ARTIFACT): $(ALL_OBJECTS)
	@echo "Linking: $@"
	arm-none-eabi-gcc -o "$@" $(ALL_OBJECTS) $(LDFLAGS)
	@echo "Finished building $@"

# Utilities
size: $(BUILD_ARTIFACT)
	arm-none-eabi-size $(BUILD_ARTIFACT)

clean:
	find .. -name '*.o' -delete
	find .. -name '*.su' -delete
	$(RM) $(BUILD_ARTIFACT) $(BUILD_ARTIFACT_NAME).map $(BUILD_ARTIFACT_NAME).list

.PHONY: all clean size
