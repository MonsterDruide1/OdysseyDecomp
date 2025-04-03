#!/usr/bin/env bash
# bash boilerplate
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
function l { # Log a message to the terminal.
    echo
    echo -e "[$SCRIPT_NAME] ${1:-}"
}

# move to the root the OdysseyHeaders repo
cd "./OdysseyHeaders"
echo "Open root of OdysseyHeaders repo"

git add -A .
git config user.name "$AUTHOR_NAME"
git config user.email $AUTHOR_MAIL
git commit -am "$MESSAGE"
git push -f -u origin $HEADER_BRANCH

echo "Updated headers successfully pushed to OdysseyHeaders repo"
