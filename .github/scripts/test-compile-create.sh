#!/usr/bin/env bash
# bash boilerplate
set -euo pipefail # strict mode
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
function l { # Log a message to the terminal.
    echo
    echo -e "[$SCRIPT_NAME] ${1:-}"
}

rm -rf src/test || true
mkdir -p src/test/single src/test/multi

for f in $(find lib src -name '*.h'); do
    if [[ $f == "lib/NintendoSDK/src/NintendoSDK"* || $f == *"/cafe/"* || $f == "lib/sead/include/prim/seadScopeGuard.h" ]]; then
        continue
    fi
    testname="${f//\//_}.cpp"  # Replace / with _
    testinclude1="${f/*\/include\//}"  # Replace .../include/ with ""
    testinclude2="${testinclude1/src\//}"  # Replace leading src/ with ""
    testinclude3="${testinclude2/lib\/al\//}"  # Replace lib/al/ with ""
    testinclude4="${testinclude3/lib\/aarch64\//aarch64\/}"  # Replace lib/aarch64/ with aarch64/
    echo "#include \"$testinclude4\"" > src/test/single/$testname
done

for f in $(find lib src -name '*.h'); do
    if [[ $f == "lib/NintendoSDK/src/NintendoSDK"* || $f == *"/cafe/"* || $f == "lib/sead/include/prim/seadScopeGuard.h" ]]; then
        continue
    fi
    testname="${f//\//_}.cpp"  # Replace / with _
    testinclude1="${f/*\/include\//}"  # Replace .../include/ with ""
    testinclude2="${testinclude1/src\//}"  # Replace leading src/ with ""
    testinclude3="${testinclude2/lib\/al\//}"  # Replace lib/al/ with ""
    testinclude4="${testinclude3/lib\/aarch64\//aarch64\/}"  # Replace lib/aarch64/ with aarch64/
    echo "#include \"$testinclude4\"" >> src/test/multi/all.cpp
done

for f in $(find src -name '*.h'); do
    if [[ $f == "lib/NintendoSDK/src/NintendoSDK"* || $f == *"/cafe/"* || $f == "lib/sead/include/prim/seadScopeGuard.h" ]]; then
        continue
    fi
    testname="${f//\//_}.cpp"  # Replace / with _
    testinclude1="${f/*\/include\//}"  # Replace .../include/ with ""
    testinclude2="${testinclude1/src\//}"  # Replace leading src/ with ""
    testinclude3="${testinclude2/lib\/al\//}"  # Replace lib/al/ with ""
    testinclude4="${testinclude3/lib\/aarch64\//aarch64\/}"  # Replace lib/aarch64/ with aarch64/
    echo "#include \"$testinclude4\"" >> src/test/multi/src.cpp
done
