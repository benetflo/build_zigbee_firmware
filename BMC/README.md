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

benji@BensLaptop:~/github_projektbuild_zigbee_ncp_firmware/BMC$ arm-none-eabi-size E45_BMC_FW_LL.elf
   text    data     bss     dec     hex filename
  10080      24    3488   13592    3518 E45_BMC_FW_LL.elf