#!/bin/bash

# Navigate to the root project directory (assuming script is in Scripts folder)
cd "$(dirname "$0")/.."

# 1. Get current git commit hash (short) and latest git tag
GIT_HASH=$(git rev-parse --short HEAD)
GIT_TAG=$(git tag --sort=-v:refname | grep "^v" | head -n 1)

# If no tag exists, set a default version (optional)
if [ -z "$GIT_TAG" ]; then
    GIT_TAG="v0.0.0"
fi

# 2. Check if there are any changes since the last commit
if ! git diff-index --quiet HEAD --; then
    echo "Error: The working tree is dirty. Please commit or stash your changes before building."
    exit 1  # Stop the build process with a non-zero exit code
fi

# 3. Handle build number increment (find latest build tag, increment)
BASE_TAG=$(echo "$GIT_TAG" | sed 's/+build\.[0-9]*//')  # Remove build suffix if any
BUILD_NUMBER=$(git tag --list "${BASE_TAG}+build.*" | sed -E "s/.*+build\.([0-9]*)/\1/" | sort -nr | head -n 1)

if [ -z "$BUILD_NUMBER" ]; then
    BUILD_NUMBER=1
else
    BUILD_NUMBER=$((BUILD_NUMBER + 1))
fi

# Create new version tag with build number
NEW_TAG="${BASE_TAG}+build.${BUILD_NUMBER}"

# Check if the exact tag already exists
if git rev-parse "$NEW_TAG" >/dev/null 2>&1; then
    echo "Tag $NEW_TAG already exists, not creating a new one."
else
    LATEST_TAG=$(git tag --points-at HEAD)
    if [ -z "$LATEST_TAG" ]; then
        # Tag the new version
        git tag "$NEW_TAG"
        echo "New tag created: $NEW_TAG"
    else
        # HEAD is already tagged, get the build number back to the original, if it is zero, it means the Release and Debug are at the same commit.
        BUILD_NUMBER=$((BUILD_NUMBER - 1))
        NEW_TAG="${BASE_TAG}+build.${BUILD_NUMBER}"
    fi
fi

# 4. Output version info to a file for the firmware section (REG_FW)
echo "Setting firmware version..."

# Combine tag and hash for firmware identification (e.g., v1.2.3-build.1-abc123)
FW_VERSION="${NEW_TAG}-${GIT_HASH}"

# Export the name as an environment variable and save it to a file for post-build
echo "FW_VERSION=\"$FW_VERSION\"" > Scripts/fw_version_env.sh

echo "Firmware version saved for post-build: $FW_VERSION"

exit 0
