# Building firmware to BMC with STM32

- Tried to clone the repo to my host machine, but no files where included, so I could not open the folder in STM32CubeIDE.

- Tried opening an existing project (my own STM32 drivers) and importing from there with the following steps

- File -> Import -> Import ac6 System Workbench for STM32 Projects

- Built the project with: Build configurations -> Build All. A new folder "Release" appeared. According to the guide, the .elf file should be located in there but it is not there.

- After building again the .elf file appeared in the Binaries folder.

- After building one more time, the .elf file appeared in the Release folder as stated in the guide. However, there are now two .elf files in the Binaries folder.

- Restarted the program and rebuilt the project to see if it removed the old file and if the binaries show up under Release after first build, the files are still there.

- Looked up when the binaries were last modified by left clicking on the file -> Show in -> Properties. The time of build does not match my systems time.

- I removed the binaries with "Clean all" under Build Configurations. After building all again, the binary appeared in the Release folder and the time of build was correct. The firmware can now be built in STM32CubeIDE, nice!

# Building firmware to BMC in the terminal

- Cloned the repo and extracted the E45_Hem_G_BMC_FW folder. Then I removed files that are not needed for terminal building:
```
.project
.mxproject
.cproject
E45_BMC_FW_LL.ioc
```

- Created the Release folder and copied over the makefile and objects.list that were generated in STM32CubeIDE to the project.

- Had to fix some paths in the Makefile to match the system, mostly just alot of "../" as prefix to path.

- Tried to make but the .o files are not compiled correctly:

```
arm-none-eabi-gcc -o "E45_BMC_FW_LL.elf" @"objects.list"   -mcpu=cortex-m0plus -T"../STM32C031G4UX_FLASH.ld" --specs=nosys.specs -Wl,-Map="E45_BMC_FW_LL.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Drivers/Button/Src/button.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Drivers/Leds/Src/leds.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/auxiliar_functions.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/comms.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/global_state.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/handlers.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/led_fsm.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/main.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/peripheral_init.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/power_fsm.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/regFile.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/ringBuf.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/stm32c0xx_it.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/syscalls.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/sysmem.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Src/system_stm32c0xx.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Core/Startup/startup_stm32c031g4ux.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_adc.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_dma.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_exti.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_gpio.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_pwr.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_rcc.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_spi.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_tim.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_usart.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Drivers/STM32C0xx_HAL_Driver/Src/stm32c0xx_ll_utils.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./Init/Src/system_init.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_adc.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_gpio.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_i2c.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_pwm.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_spi.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_systick.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_tim.o: No such file or directory
/usr/lib/gcc/arm-none-eabi/13.2.1/../../../arm-none-eabi/bin/ld: cannot find ./sdk/Src/SDK_uart.o: No such file or directory
collect2: error: ld returned 1 exit status
make: *** [makefile:68: E45_BMC_FW_LL.elf] Error 1
```
- Created a new makefile, the project now compiles:

```
# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU flags
MCU = -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft
CFLAGS = $(MCU) -O2 -Wall -ffunction-sections -fdata-sections --specs=nano.specs
LDFLAGS = -TSTM32C031G4UX_FLASH.ld -Wl,--gc-sections -static --specs=nosys.specs
LIBS = -lc -lm

# Sources
SRC = $(wildcard Core/Src/*.c \
              Core/Drivers/Leds/Src/*.c \
              Core/Drivers/Button/Src/*.c \
              Drivers/STM32C0xx_HAL_Driver/Src/*.c \
              Init/Src/*.c \
              sdk/Src/*.c)

# Object files
OBJ = $(SRC:.c=.o)

# Output
TARGET = E45_BMC_FW_LL

# Rules
all: $(TARGET).elf

# Compile .c -> .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link .o -> ELF, generate binary
$(TARGET).elf: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $@
	$(SIZE) $@
	$(OBJCOPY) -O binary $@ $(TARGET).bin

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).bin

```

- However the ELF file is a little bit smaller than the one that was build in STM32CubeIDE. Used Claude to get a working makefile but now the file is 39k, original is 34k

- Used "scp" to copy the STM32CubeIDE compiled binary to WSL. Then i used **"arm-none-eabi-size"** to compare the binaries sizes:

| Parameter | STM32 binary | Terminal binary |
|-----------|--------------|-----------------|
|text       |10248         |10080            |
|data       |24            |24               |
|bss        |3488          |3488             |
|dec        |13760         |13592            |
|hex        |35c0          |3518             |

Some differences here. 168 more bytes text(code) in the STM32 compiled code.

- Compared the files ones again with:
```
arm-none-eabi-nm -S --size-sort  STM32.elf
```
- Used [diffchecker](https://www.diffchecker.com/text-compare/) to compare the results. There are indeed some differences, most involving power supply functions.

- I checked the files of one of the missing functions, but the code is there in my terminal. Maybe the linker is removing it for optimization? I tried to remove the linker flag "--gc-sections" but that gave a warning. The firmware got to large to fit in MCU flash.

- Found the C file where most of the missing functions are located. Tried to add __attribute__((used)) before every function that was missing in the terminal compiled binary. Compiled again, still the same size so checked again with:
```
arm-none-eabi-size  STM32.elf
```

- I went back and added this to the **power_fsm.c** file:
```
__attribute__((used, noinline))
```
The terminal firmware has now grown in text and hex!

- Now I'll try to add "__attribute__((used, noinline))" to the rest that are missing. But first I have to find out what's missing:

```
arm-none-eabi-nm -n STM32.elf > stm32.syms
arm-none-eabi-nm -n E45_BMC_FW_LL.elf > term.syms
```
```
awk '{print $3}' stm32.syms | sort > stm32.names
awk '{print $3}' term.syms | sort > term.names
```
```
comm -23 stm32.names term.names > missing_in_terminal.txt
```
We now have a text file with the missing code:
```
L_EXTI_SetEXTISource
LL_TIM_OC_SetMode
LL_USART_SetBaudRate
OFFSET_TAB_CCMRx
awk '{print $3}' stm32.syms | sort > stm32.names
awk '{print $3}' term.syms | sort > term.names
RCC_GetHCLKClockFreq
RCC_GetPCLK1ClockFreq
SDK_ADC_StartConversion
SHIFT_TAB_OCxx
__NVIC_SetPriority
__NVIC_SystemReset
comms_calcChkSum
rb_incBufReadPtr
rb_incBufWritePtr
reg_CONFIG_handler
shutdownActions
```

- I tried adding the same compile flags as STM32CubeIDE does, the file now got larger, I will compare again. Nothing was missing! I will try checking the other way around if there is something new in the terminal compiled file vs the STM32CubeIDE file:

```
comm -13 stm32.names term.names > missing_in_STM32.txt
```
These do not exist in the STM32CubeIDE compiled version but they do exist in the one compiled in the terminal.
```
__TMC_END__
__fini_array_end
__fini_array_start
deregister_tm_clones
register_tm_clones
```

- Tried removing -static but the size is still larger than the STM32 version. Doesn't seem to affect the size

- I tried different optimization flags (Os, Og, O1, O2, O2, Ofast) it changed the size of the file but was not exactly the same. Probably different flags in STM32CubeIDE, I should check compile flag settings in STM32CubeIDE.

# Next steps try flashing firmware

- I currently have:
```
Terminal version size:           40K
STM32CubeIDE version size :      36K

//Difference could be hardware guy build with earlier STM32CubeIDE version or the extras?
```

Terminal version as extras:
```
__TMC_END__
__fini_array_end
__fini_array_start
deregister_tm_clones
register_tm_clones

# Note: Terminal build adds extra GCC runtime symbols
# (__TMC_END__, __fini_array_start/end, deregister_tm_clones, register_tm_clones)
# These are bookkeeping symbols only and do not affect firmware functionality.
```

