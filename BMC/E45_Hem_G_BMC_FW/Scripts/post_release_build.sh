#!/bin/bash

# Source the environment variable file created by the pre-build script
FW_VERSION=$(grep 'FW_VERSION=' "$(dirname "$0")/fw_version_env.sh" | cut -d'=' -f2 | tr -d '"')

# Remove the fw_version_env.sh file after reading
rm -f "$(dirname "$0")/fw_version_env.sh"

# Ensure the firmware version is set from the pre-build script
if [ -z "$FW_VERSION" ]; then
  echo "Firmware version not found. Ensure pre_build.sh is executed."
  exit 1
fi

# Navigate to the root project directory (the parent directory of the Scripts folder)
cd "$(dirname "$0")/.."

# Get the project name (using the current directory name)
PROJECT_NAME=$(basename "$PWD")  # Get the current directory name as the project name

# Get the absolute path of the build directory
BUILD_DIR=$(realpath "$(dirname "$0")/../Release")  # Get the absolute path of the build folder

# Set the ELF and BIN file paths
ELF_FILE="${BUILD_DIR}/${PROJECT_NAME}.elf"  # Ensure this matches the actual ELF file name
BIN_FILE="${BUILD_DIR}/${PROJECT_NAME}.bin"  # Ensure this matches the actual BIN file name

# Debug output for the paths
echo "Looking for ELF file: $ELF_FILE"

# Write the firmware version into REG_FW section
FW_SECTION=".FW"

# Create a temporary binary file for the firmware version (ensure it's 48 bytes)
FW_VERSION_BIN=$(mktemp)
printf "%-48s" "$FW_VERSION" | dd of="$FW_VERSION_BIN" bs=1 count=48 conv=notrunc

# Check if the section exists before updating
if arm-none-eabi-readelf -S "$ELF_FILE" | grep -q "$REG_FW_SECTION"; then
  echo "Updating REG_FW section: $REG_FW_SECTION"
    
  # Proceed to update the section
  arm-none-eabi-objcopy --update-section "$FW_SECTION=$FW_VERSION_BIN" "$ELF_FILE" || {
    echo "Failed to update ELF file: $ELF_FILE"
    exit 1
  }
else
  echo "Error updating ELF file: $ELF_FILE"
fi

echo "Firmware version ($FW_VERSION) written to the .FW section in the ELF file."

# Optionally, generate the updated binary file if needed
arm-none-eabi-objcopy -O binary "$ELF_FILE" "$BIN_FILE"

echo "Generated ($BIN_FILE) file."

# Clean up
rm -f "$FW_VERSION_BIN"
