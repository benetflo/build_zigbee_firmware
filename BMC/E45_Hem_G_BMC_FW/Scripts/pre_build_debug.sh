#!/bin/bash

# Navigate to the root project directory (assuming script is in Scripts folder)
cd "$(dirname "$0")/.."

# 0. Pull tags from the remote repository (configurable)
if [ "${PULL_TAGS:-1}" -eq 1 ]; then
    echo "Fetching latest tags from remote..."
    if ! git fetch --tags; then
        echo "Warning: Failed to fetch tags from remote. Proceeding with local tags."
    fi
fi

# 1. Get current branch name
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)

# 2. Get current git commit hash (short) and latest git tag
GIT_HASH=$(git rev-parse --short HEAD)
GIT_TAG=$(git tag --sort=-v:refname | head -n 1)

# If no tag exists, set a default version
if [ -z "$GIT_TAG" ]; then
    GIT_TAG="v0.0.0"
fi

# Remove build suffix if present
GIT_TAG=$(echo "$GIT_TAG" | sed 's/+build\.[0-9]*//')

# Extract version parts from the tag (assuming tag starts with 'v')
GIT_TAG_NO_V="${GIT_TAG#v}"

MAJOR=$(echo "$GIT_TAG_NO_V" | cut -d. -f1)
MINOR=$(echo "$GIT_TAG_NO_V" | cut -d. -f2)
PATCH=$(echo "$GIT_TAG_NO_V" | cut -d. -f3)

# 3. Generate a timestamp for the debug number
DEBUG_NUMBER=$(date +"%Y%m%d%H%M%S")  # Format: YYYYMMDDHHMMSS

# 4. Check if the working directory is dirty (i.e., has uncommitted changes)
DIRTY=""
DEV=""

if ! git diff-index --quiet HEAD --; then
    DIRTY="-dirty"
fi

# Working directory is clean or dirty
if [ "$CURRENT_BRANCH" == "main" ]; then
    echo "On main branch. Incrementing patch version..."
    PATCH=$((PATCH + 1))
    GIT_TAG="v${MAJOR}.${MINOR}.${PATCH}"
    
    # Ensure the tag doesn't already exist
    if git rev-parse "$GIT_TAG" >/dev/null 2>&1; then
        echo "Tag '$GIT_TAG' already exists. Skipping tag creation."
    else
        echo "Creating new tag: $GIT_TAG"
        if git tag "$GIT_TAG"; then
            echo "Successfully created tag: $GIT_TAG"
            
            # Push the tag to the remote if configured
            if [ "${PUSH_TAGS:-1}" -eq 1 ]; then
                if git push origin "$GIT_TAG"; then
                    echo "Tag '$GIT_TAG' successfully pushed to remote."
                else
                    echo "Warning: Failed to push tag '$GIT_TAG' to remote."
                fi
            else
                echo "Skipping tag push due to PUSH_TAGS=0."
            fi
        else
            echo "Error: Failed to create tag '$GIT_TAG'."
            exit 1
        fi
    fi
else
    echo "Not on main branch. Adding '-dev' to version."
    DEV="-dev"
fi

# 5. Get the developer ID (username)
DEV_ID=$(whoami)

# Ensure DEV_ID is exactly 6 characters
if [ ${#DEV_ID} -gt 6 ]; then
    # Truncate if longer than 6 characters
    DEV_ID=$(echo "$DEV_ID" | cut -c1-6)
else
    # Pad with '0' if shorter than 6 characters
    DEV_ID=$(printf "%-6s" "$DEV_ID" | tr ' ' '0')
fi

# 6. Combine the tag, incremental pre-release number, git hash, dirty state, and developer ID
FW_VERSION="${GIT_TAG}${DEV}+${GIT_HASH}${DIRTY}-${DEV_ID}-${DEBUG_NUMBER}"

# 7. Export the name as an environment variable and save it to a file for post-build
echo "FW_VERSION=\"$FW_VERSION\"" > Scripts/fw_version_env.sh

echo "Firmware version saved for post-build: $FW_VERSION"

exit 0
