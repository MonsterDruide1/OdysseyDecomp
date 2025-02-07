#!/usr/bin/env bash
# bash boilerplate
set -euo pipefail # strict mode
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
function l { # Log a message to the terminal.
    echo
    echo -e "[$SCRIPT_NAME] ${1:-}"
}

# Delete everything within the headers repo to get a fresh start
rm -r $DESTINATION_PATH/*

# Copy libraries
mkdir $DESTINATION_PATH/NintendoSDK $DESTINATION_PATH/agl $DESTINATION_PATH/al $DESTINATION_PATH/eui $DESTINATION_PATH/sead
cp -r ./lib/NintendoSDK/include/* $DESTINATION_PATH/NintendoSDK
cp -r ./lib/aarch64 $DESTINATION_PATH/
cp -r ./lib/agl/include/* $DESTINATION_PATH/agl
rsync -a --prune-empty-dirs --include '*/' --include '*.h' --exclude '*' lib/al/ $DESTINATION_PATH/al
cp -r ./lib/eui/include/* $DESTINATION_PATH/eui
cp -r ./lib/sead/include/* $DESTINATION_PATH/sead

# Copy headers of game
mkdir $DESTINATION_PATH/game
rsync -a --prune-empty-dirs --include '*/' --include '*.h' --exclude '*' src/ $DESTINATION_PATH/game

# Copy CMakeLists.txt for header only library
cp ./.github/files/HeadersCMakeLists.txt $DESTINATION_PATH/CMakeLists.txt

# Make all contents of every class public
grep -rli 'private:' * | xargs -i@ sed -i 's/private:/public:/g' @
grep -rli 'protected:' * | xargs -i@ sed -i 's/protected:/public:/g' @
