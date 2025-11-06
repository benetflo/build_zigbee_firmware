# Building firmware to BMC with STM32

- Tried to clone the repo to my host machine, but no files where included, so I could not open the folder in STM32CubeIDE.

- Tried opening an existing project (my own STM32 drivers) and importing from there with the following steps

- File -> Import -> Import ac6 System Workbench for STM32 Projects

- Built the project with: Build configurations -> Build All. A new folder "Release" appeared. According to the guide, the .elf file should be located in there but it is not there.

- After building again the .elf file appeared in the Binaries folder.

- After building one more time, the .elf file appeared in the Release folder as stated in the guide. However, there are now two .elf files in the Binaries folder.

- Restarted the program and rebuilt the project to see if it removed the old file and if the binaries show up under Release after first build, the files are still there.

- Looked up when the binaries were last modified by left clicking on the file -> Show in -> Properties. The time does not match my systems time.

- I removed the binaries with "Clean all" under Build Configurations. After building all again, the binary appeared in the Release folder and the time of build was correct. The firmware can now be built in STM32CubeIDE, nice!


