################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include sdk/Src/subdir.mk
-include Init/Src/subdir.mk
-include Drivers/STM32C0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Drivers/Leds/Src/subdir.mk
-include Core/Drivers/Button/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

#OPTIONAL_TOOL_DEPS := \
#$(wildcard ../makefile.defs) \
#$(wildcard ../makefile.init) \
#$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := E45_BMC_FW_LL
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
E45_BMC_FW_LL.elf \

MAP_FILES += \
E45_BMC_FW_LL.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
E45_BMC_FW_LL.list \

# Hitta alla .c filer rekursivt
SOURCES := $(shell find ../Core ../Drivers ../Init ../sdk -name '*.c')

# Hitta alla .s filer
ASM_SOURCES := $(shell find ../Core/Startup -name '*.s')

# Generera objektfil-namn (behåll sökväg)
C_OBJECTS := $(SOURCES:.c=.o)
ASM_OBJECTS := $(ASM_SOURCES:.s=.o)
ALL_OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)


# Compilation flags
CFLAGS = -mcpu=cortex-m0plus -O2 -Wall -ffunction-sections -fdata-sections \
         -I../Core/Inc \
         -I../Drivers/STM32C0xx_HAL_Driver/Inc \
         -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy \
         -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include \
         -I../Drivers/CMSIS/Include \
         -I../sdk/Inc \
         -I../Init/Inc \
         -I../Core/Drivers/Button/Inc \
         -I../Core/Drivers/Leds/Inc \
         -DSTM32C031xx -DUSE_FULL_LL_DRIVER

# C compile rule
%.o: %.c
	@echo 'Compiling: $<'
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
	@echo ' '
# Assembly rule
%.o: %.s
	@echo 'Assembling: $<'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -x assembler-with-cpp -c $< -o $@
	@echo ' '

# All Target
all: main-build

# Main-build Target
main-build: E45_BMC_FW_LL.elf secondary-outputs

# Tool invocations
E45_BMC_FW_LL.elf: $(ALL_OBJECTS)
	@echo 'Linking: $@'
	arm-none-eabi-gcc -o "$@" $(ALL_OBJECTS) -mcpu=cortex-m0plus \
	-T"../STM32C031G4UX_FLASH.ld" --specs=nosys.specs \
	-Wl,-Map="E45_BMC_FW_LL.map" -Wl,--gc-sections -static \
	--specs=nano.specs -mfloat-abi=soft -mthumb \
	-Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

E45_BMC_FW_LL.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "E45_BMC_FW_LL.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) E45_BMC_FW_LL.elf E45_BMC_FW_LL.list E45_BMC_FW_LL.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
