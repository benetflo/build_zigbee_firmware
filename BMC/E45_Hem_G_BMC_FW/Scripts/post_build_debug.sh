#!/bin/bash

# Source the environment variable file created by the pre-build script
FW_VERSION_FILE="$(dirname "$0")/fw_version_env.sh"

if [ ! -f "$FW_VERSION_FILE" ] || ! grep -q 'FW_VERSION=' "$FW_VERSION_FILE"; then
  echo "fw_version_env.sh not found or missing FW_VERSION variable."
  exit 1
fi

FW_VERSION=$(grep 'FW_VERSION=' "$FW_VERSION_FILE" | cut -d'=' -f2 | tr -d '"')

# Remove the fw_version_env.sh file after reading
rm -f "$FW_VERSION_FILE"

if [ -z "$FW_VERSION" ]; then
  echo "Firmware version not found. Ensure pre_build.sh is executed."
  exit 1
fi

cd "$(realpath "$(dirname "$0")/..")"

PROJECT_NAME=$(basename "$PWD")
# BUILD_DIR=$(realpath "$(dirname "$0")/../Debug")
BUILD_DIR=$(realpath "$(dirname "$0")/../$PROJECT_NAME/Debug")

ELF_FILE="${BUILD_DIR}/${PROJECT_NAME}.elf"
BIN_FILE="${BUILD_DIR}/${PROJECT_NAME}.bin"
FW_SECTION=".FW"

FW_VERSION_BIN=$(mktemp) || { echo "Failed to create temporary file."; exit 1; }
printf "%-48s" "$FW_VERSION" | dd of="$FW_VERSION_BIN" bs=1 count=48 conv=notrunc

if arm-none-eabi-readelf -S "$ELF_FILE" | grep -q "$FW_SECTION"; then
  echo "Updating REG_FW section: $FW_SECTION"
  if ! arm-none-eabi-objcopy --update-section "$FW_SECTION=$FW_VERSION_BIN" "$ELF_FILE"; then
    echo "Failed to update ELF file."
    rm -f "$FW_VERSION_BIN"
    exit 1
  fi
else
  echo "Section $FW_SECTION not found in ELF file."
  rm -f "$FW_VERSION_BIN"
  exit 1
fi

echo "Firmware version $FW_VERSION written to the .FW section in the ELF file."

if ! arm-none-eabi-objcopy -O binary "$ELF_FILE" "$BIN_FILE"; then
  echo "Failed to generate binary file."
  rm -f "$FW_VERSION_BIN"
  exit 1
fi

echo "Generated binary file: $BIN_FILE"

rm -f "$FW_VERSION_BIN"
