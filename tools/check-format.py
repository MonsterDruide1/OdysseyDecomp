#!/usr/bin/env python3

import argparse
import csv
import os
import stat
import re
import subprocess
from functools import cache

from common import setup_common as setup
from common.util import utils

# ------
# CHECKS
# ------

issueFound = False
runAllChecks = False

def FAIL(message, line, path):
    print("Offending file:", os.path.relpath(path, os.getcwd()))
    print("Line:", line)
    print(message)
    print()
    global issueFound
    issueFound = True

def CHECK(cond, line, message, path):
    if not cond(line):
        FAIL(message, line, path)
        return True
    return False

# Common

def common_no_namespace_qualifiers(c, path):
    nest_level = []
    for line in c.splitlines():
        if "http://" not in line:
            line = line[0:line.find("//")] if "//" in line else line
        if line.startswith("using namespace"):
            match = re.search(r"^using namespace ([^;\s]+);$", line)
            if CHECK(lambda a: match, line,
                     "Unexpected \"using namespace\" line: should follow format \"using namespace xy;\"", path): return
            continue
        if CHECK(lambda a: a.rfind("namespace") in [-1, 0], line,
                 "\"namespace\" must only be listed at the start of a line!", path): return

        if line.startswith("namespace"):
            match = re.search(r"^namespace ([^{\s]*) ?{$", line)
            if CHECK(lambda a: match, line, "Unexpected namespace line: should follow format \"namespace xy {\"",
                     path): return
            nest_level.append(match.group(1))
            # can be "" for "namespace {" and "nn::g3d" for double/triple/... namespaces
            continue

        allowed_namespaces = []
        for l in nest_level:
            if l != "" and l != None:
                if "::" not in l:
                    allowed_namespaces.append(l)
                else:
                    allowed_namespaces += l.split("::")

        parts = re.split("([{}])", line)
        for x in parts:
            if x == "{":
                nest_level.append(None)
                continue
            if x == "}":
                if nest_level[-1] != None:  # is closing namespace
                    if CHECK(lambda a: a == "}", line.rstrip(), "Closing namespace expects only \"}\" in line!",
                             path): return
                del nest_level[-1]
                continue

            matches = re.findall(r"[\(,\s]([^\(,\s]+::)+[^\(,\s]+", x)
            for match in matches:
                match = match[0:-2]
                # examples: "sead", "al", "nn::g3d"
                if CHECK(lambda a: match not in allowed_namespaces, line, match + " should be omitted here!",
                         path): return

    if len(nest_level) != 0:
        print("ERROR: nest_level not empty at end of the file!")
        print("nest_level", nest_level)
        if not runAllChecks:
            exit(1)

@cache
def get_includes():
    def get_files(path):
        fs = []
        for root, _, files in os.walk(path):
            for file in files:
                file_path = os.path.join(root, file)
                fs.append(os.path.relpath(file_path, path))
        return fs

    cpp_files = ["concepts", "coroutine", "any", "bitset", "chrono", "compare", "csetjmp", "csignal", "cstdarg",
                 "cstddef", "cstdlib", "ctime", "expected", "functional", "initializer_list", "optional",
                 "source_location", "tuple", "type_traits", "typeindex", "typeinfo", "utility", "variant", "version",
                 "memory", "memory_resource", "new", "scoped_allocator", "cfloat", "cinttypes", "climits", "cstdint",
                 "limits", "stdfloat", "cassert", "cerrno", "exception", "stacktrace", "stdexcept", "system_error",
                 "cctype", "charconv", "cstring", "cuchar", "cwchar", "cwctype", "format", "string", "string_view",
                 "array", "deque", "flat_map", "flat_set", "forward_list", "list", "map", "mdspan", "queue", "set",
                 "span", "stack", "unordered_map", "unordered_set", "vector", "iterator", "generator", "ranges",
                 "algorithm", "execution", "bit", "cfenv", "cmath", "complex", "numbers", "numeric", "random", "ratio",
                 "valarray", "clocale", "codecvt", "locale", "text_encoding", "cstdio", "fstream", "iomanip", "ios",
                 "iosfwd", "iostream", "istream", "ostream", "print", "spanstream", "sstream", "streambuf", "strstream",
                 "syncstream", "filesystem", "regex", "atomic", "barrier", "condition_variable", "future",
                 "hazard_pointer", "latch", "mutex", "rcu", "semaphore", "shared_mutex", "stop_token", "thread",
                 "stdatomic.h"]
    sead_files = get_files(project_root / 'lib' / 'sead' / 'include')
    nintendo_sdk_files = get_files(project_root / 'lib' / 'NintendoSDK' / 'include')
    agl_files = get_files(project_root / 'lib' / 'agl' / 'include')
    aarch_files = get_files(project_root / 'lib' / 'aarch64')
    eui_files = get_files(project_root / 'lib' / 'eui' / 'include')
    al_files = [a for a in get_files(project_root / 'lib' / 'al') if a.endswith(".h")]
    game_files = [a for a in get_files(project_root / 'src') if a.endswith(".h")]

    angled_includes = cpp_files + aarch_files + nintendo_sdk_files + sead_files + agl_files + eui_files
    al_includes = al_files
    game_includes = game_files

    return angled_includes, al_includes, game_includes

def common_include_order(c, path, is_header):
    lines = c.split("\n")
    if is_header:
        if CHECK(lambda a: a == "#pragma once", lines[0], "Headers must start with \"#pragma once\"!", path): return
        del lines[0]
        if CHECK(lambda a: a == "", lines[0], "\"#pragma once\" must be followed by exactly one empty line!",
                 path): return
        del lines[0]
    elif not path.endswith("src/System/Init.cpp"):
        # hardcoded exception: Init.cpp contains C functions and no header to relate to
        if "src/" in path:
            rel_path = path.split("src/")[-1]
        elif "al/" in path:
            rel_path = path.split("al/")[-1]
        elif "include/" in path:
            rel_path = path.split("include/")[-1]
        else:
            rel_path = path
        header_line = "#include \"" + rel_path[0:-3] + "h\""
        if CHECK(lambda a: a == header_line, lines[0],
                 "Source files must start with including respective header in double quotes (here: " + header_line + ")!",
                 path): return
        del lines[0]
        if CHECK(lambda a: a == "", lines[0], "\"" + header_line + "\" must be followed by exactly one empty line!",
                 path): return
        del lines[0]

    include_lines = []
    non_empty_include_lines = []
    end_of_includes = False
    for line in lines:
        if line.startswith("#include"):
            if CHECK(lambda a: not end_of_includes, line, "Includes have to be listed at the very top of the file!",
                     path): return
            include_lines.append(line)
            non_empty_include_lines.append(line)
        elif line == "" and not end_of_includes:
            include_lines.append(line)
        else:
            end_of_includes = True

    angled_includes, al_includes, game_includes = get_includes()

    if len(non_empty_include_lines) != len(set(non_empty_include_lines)):
        FAIL("Found duplicate includes!", -1, path)

    order = -1  # -1=none (after initial newline) ; 0=angled (sead, agl, nn, eui) ; 1=al ; 2=game ; -2,-3,-4=newline after respective section (angled, al, game)
    for line in include_lines:
        if line == "":
            if CHECK(lambda a: order in [0, 1, 2], line,
                     "Unexpected newline in includes! Please re-read the contribution guide and format accordingly.",
                     path): return
            order = -order - 2
            continue

        if line.startswith("#include <") and line.endswith(">"):
            file = line[10:-1]
            if CHECK(lambda a: order in [-1, 0], line,
                     "Angled \"#includes <...>\" can only appear in the first block of includes!", path): return
            if CHECK(lambda a: file in angled_includes, line, "This file is not allowed to be included with <>!",
                     path): return
            order = 0
        elif line.startswith("#include \"") and line.endswith("\""):
            file = line[10:-1]
            if file in al_includes:
                if CHECK(lambda a: order in [-1, -2, 1], line,
                         "Wrong order for includes: Found \"al\"-include outside of expected block!", path): return
                order = 1
            elif file in game_includes:
                if CHECK(lambda a: order in [-1, -2, -3, 2], line,
                         "Wrong order for includes: Found \"game\"-include outside of expected block!", path): return
                order = 2
            else:
                FAIL("This file is not allowed to be included with \"file\"!", line, path)
                return
        else:
            FAIL("Unknown include format", line, path)
            return

    CHECK(lambda a: order in [-1, -2, -3, -4], "not applicable", "Empty line expected after includes!", path)

def common_newline_eof(c, path):
    CHECK(lambda a: a == "", c.split("\n")[-1], "Files should end with a newline!", path)

def common_sead_types(c, path):
    FORBIDDEN_TYPES = ["int", "float", "short", "long", "double", "char16_t"]
    for line in c.splitlines():
        for t in FORBIDDEN_TYPES:
            index = 0
            while index < len(line):
                index = line.find(t, index)
                if index == -1:
                    break
                if index > 0 and line[index - 1].isalnum():
                    index += 1
                    continue
                if index + len(t) < len(line) and line[index + len(t)].isalnum():
                    index += 1
                    continue
                FAIL(
                    "Forbidden type used: " + t + ". Use equivalent of <basis/seadTypes.h> instead (f32, s32, u32, ...)",
                    line, path)
                return

def common_void_params(c, path):
    for line in c.splitlines():
        if "(void)" in line:
            FAIL("Function parameters should be empty instead of \"(void)\"!", line, path)
            return

def common_const_type(c, path):
    for line in c.splitlines():
        line = line.split("//")[0]
        index = 0
        while index < len(line):
            index = line.find("const", index)
            if index == -1:
                break
            if index > 0 and line[index - 1].isalnum():  # const is just part of a longer string
                index += 1
                continue
            if index >= 0 and line[index + len("const")] in ['*', '&']:
                FAIL("Const must be placed before the type: const T* or const T&", line, path)
                index += 1
                continue
            index += 1

def common_this_prefix(c, path):
    for line in c.splitlines():
        if 'this->' in line:
            FAIL("this-> is not allowed!", line, path)

def common_consistent_float_literals(c, path):
    for line in c.splitlines():
        index = line.find(".f")
        if index != -1 and not line[index + 2].isalpha():
            FAIL(" '.f' is not allowed, use '.0f' instead!", line, path)

def common_sead_math_template(c, path):
    for line in c.splitlines():
        if "<f32>" in line or "<s32>" in line or "<u32>" in line or "<f64>" in line or "<s64>" in line or "<u64>" in line:
            if "sead" not in line and "Matrix43" not in line:
                continue
            if "using" in line or "typedef" in line:
                continue
            if "sead::Buffer" in line:  # probably needs more exceptions at some point
                continue
            if "Vector3CalcCommon" in line:
                continue
            FAIL("Use short sead types: sead::Vector3f, sead::Mathi and similar!", line, path)

def common_string_finder(c, path):
    string_table = get_string_table()

    for line in c.splitlines():
        if "#include" in line:
            continue
        if "extern \"C\"" in line:
            continue
        if "__asm__" in line:
            continue
        if "asm volatile" in line:
            continue
        if "//" in line:
            continue

        matches = re.findall(r'(u?".*?")', line)

        for match in matches:
            if not match.startswith("u"):
                # Remove quotes from utf8 strings
                match = match[1:-1]
            if len(match) < 2:
                continue
            found = False
            for x in string_table:
                if match == x[1]:
                    found = True
                    break
            if not found:
                FAIL("String not found in binary: \""+match+"\"", line, path)

def common_const_reference(c, path):
    for line in c.splitlines():
        if "& " in line and line[line.find("& ") - 1] != "&" and line[line.find("& ") - 1] != " " and "CLASS&" not in line:
            if ("const" not in line or line.find("& ") < line.find("const ")) and ("for" not in line or " : " not in line) and ("operator->" not in line):
                FAIL("References must be const!", line, path)

def common_self_other(c, path, is_header):
    lines = c.splitlines()
    for i, line in enumerate(lines):
        if (("attackSensor(" in line and "void HitSensor" not in line) or "receiveMsg(" in line) and (is_header or "::" in line) and (("self" not in line and "self" not in lines[i + 1]) or "other" not in line) and "Library/HitSensor/HitSensorKeeper.h" not in path:
            FAIL("'attackSensor' and 'receiveMsg' should have 'self' and 'other' params!", line, path)
            return

# Header files

def header_sorted_visibility(c, path):
    is_in_struct = False
    visibilities_ordered = ["public:", "protected:", "private:"]
    nest_level = [-2]  # -2 = outside of class ; -1 = inside class ; 0 = public ; 1 = protected ; 2 = private
    should_start_class = False
    for line in c.splitlines():
        if re.search(r"^\s*struct.*{", line):
            is_in_struct = True
        if "};" in line:
            is_in_struct = False
        line = line[0:line.find("//")] if "//" in line else line
        if line.endswith("\\"): line = line[0:-1]
        line = line.strip()
        if line not in visibilities_ordered:
            header_check_line(line, path, nest_level[-1], should_start_class, is_in_struct)
        if "{" in line and "}" in line:
            if CHECK(lambda a: a.count("{") == a.count("}") or (a.startswith("{") and a.endswith("}};")), line,
                     "Unbalanced \"{\" and \"}\" in the same line! (exception: end of brace-initialized array)",
                     path): return
            if line.startswith("{") and line.endswith("}};"):
                del nest_level[-1]
            should_start_class = False
            continue

        if line.startswith("class ") and not line.endswith(";"):
            should_start_class = True

        if CHECK(lambda a: [b for b in visibilities_ordered if b in a and a != b] == [], line,
                 "visibility modifier must be its own line!", path): return
        if CHECK(lambda a: a.count("{") + a.count("}") <= 1, line, "Only one \"{\" and \"}\" is allowed per line!",
                 path): return

        if line in visibilities_ordered:
            i = visibilities_ordered.index(line)
            if CHECK(lambda a: i > nest_level[-1] or (i == 2 and nest_level[-1] == 2), line,
                     "Wrong order of visibilities: Must be public, protected, private!", path): return
            if nest_level[
                -1] == -2:  # outside of class, only seen in SubActorKeeper.h in a macro definition - ignore then
                continue
            nest_level[-1] = i
            continue
        elif "{" in line:
            nest_level.append(-2 if not should_start_class else -1)
            should_start_class = False
        elif "}" in line:
            del nest_level[-1]

    if len(nest_level) != 1:
        print("ERROR: nest_level not empty at end of the file!")
        print("nest_level", nest_level)

        if not runAllChecks:
            exit(1)

def header_check_line(line, path, visibility, should_start_class, is_in_struct):

    if is_in_struct:
        if re.search(r"\w+[\*&]*\s+m[A-Z]", line):
            FAIL("Struct member variables should be formatted as noPrefixCamelCase!", line, path)

    if visibility == -2:  # outside of class/struct/...
        if (line.startswith("class") and (not line.endswith(";") or "{" in line)) or should_start_class:
            if ": " in line and not ": public" in line and not ": virtual public" in line:
                FAIL("All superclasses must be public!", line, path)
            if should_start_class and not ": " in line and not line.startswith("public") and not line.startswith(
                    "virtual public"):
                FAIL("All superclasses must be public!", line, path) 

            if line.startswith("class") and "{" in line and ": " in line:
                index = 0
                while index < len(line):
                    index = line.find(",", index + 1)
                    if index == -1: break
                    if index < line.find(": "): continue
                    if index != line.find(", public", index) and index != line.find(", virtual public", index):
                        FAIL("All superclasses must be public!", line, path)
    elif visibility == -1:  # inside class, but not in a visibility block
        allowed = line in ["", "};"] or line.startswith("SEAD_SINGLETON_DISPOSER") or line.startswith(
            "SEAD_RTTI_BASE") or line.startswith("SEAD_RTTI_OVERRIDE")
        CHECK(lambda a: allowed, line,
              "Inside class, but not in a visibility block, only empty lines and closing brace allowed!", path)
    elif visibility == 0:  # public
        if "(" in line:  # function
            function_name = line.split("(")[-2].split(" ")[-1]
            CHECK(lambda a: not function_name.endswith("_"), line,
                  "Functions ending with an underscore are either protected or private!", path)
    elif visibility == 2:  # private
        if line == "};" or line == "" or line == "union {" or line.startswith("struct"): return
        if "(" in line and ")" in line: return
        newline = line
        if "=" in line:
            newline = line.split("=")[0].strip()
        elif line.endswith(";"):
            newline = line.split(";")[0].strip()
        else:
            FAIL("Unknown private line!", line, path)

        if newline.endswith("]"):
            newline = newline.split("[")[0].strip()

        var_name = newline.split(" : ")[0].split(" ")[-1]
        var_type = " ".join(newline.split(" ")[0:-1])

        if var_type.startswith("enum") or var_type.startswith("friend"):
            return  # Allow enum and friend class

        PREFIXES = ["pad", "field", "unk", "gap", "_", "filler"]

        if var_type.startswith("static"):
            CHECK(lambda a: var_name.startswith("s") and var_name[1].isupper(), line,
                  "Static member variables must be prefixed with `s`!", path)
        else:
            allowed_name = (var_name.startswith("m") and var_name[1].isupper()) or any(
                [var_name.startswith(p) for p in PREFIXES])
            CHECK(lambda a: allowed_name, line, "Member variables must be prefixed with `m`!", path)

        if var_type == "bool":
            BOOL_PREFIXES = ["mIs", "mHas", "mAlways"]
            allowed_name = any(
                [var_name.startswith(p) and (var_name[len(p)].isupper() or var_name[len(p)].isdigit()) for p in
                 BOOL_PREFIXES]) or any([var_name.startswith(p) for p in PREFIXES])
            if path.endswith("ByamlWriterData.h") and var_name == "mValue": return
            CHECK(lambda a: allowed_name, line, "Boolean member variables must start with `mIs` or `mHas`!", path)

def header_no_offset_comments(c, path):
    for line in c.splitlines():
        if "// 0x" in line or "// _" in line:
            FAIL("Offset comments are not allowed in headers!", line, path)

def header_lowercase_member_offset_vars(c, path):
    for line in c.splitlines():
        if re.search(r"\s(field|gap|filler|pad)?_[0-9a-z]*[A-Z]", line):
            CHECK(lambda a: "#define" in a, line, "Characters in the names of offset variables need to be lowercase!", path)


# Source files

def source_no_raw_auto(c, path):
    for line in c.splitlines():
        if "auto" in line and not "auto*" in line and not "auto&" in line and not " it " in line and "node " not in line and ".end()" not in line:
            FAIL("Raw use of auto isn't allowed! Please use auto* or auto& instead", line, path)

def source_no_nerve_make(c, path):
    for line in c.splitlines():
        if "NERVE_MAKE(" in line:
            FAIL("Use of NERVE_MAKE is not allowed. Use NERVES_MAKE_[NO]STRUCT instead.", line, path)
            return

# -----
# UTILS
# -----

def check_source(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    common_include_order(c, path, False)
    common_sead_types(c, path)
    common_void_params(c, path)
    common_const_type(c, path)
    common_this_prefix(c, path)
    common_string_finder(c, path)
    common_sead_math_template(c, path)
    source_no_nerve_make(c, path)
    common_const_reference(c, path)
    source_no_raw_auto(c, path)
    common_self_other(c, path, False)
    common_consistent_float_literals(c, path)

def check_header(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    common_include_order(c, path, True)
    common_sead_types(c, path)
    common_void_params(c, path)
    common_const_type(c, path)
    common_sead_math_template(c, path)
    header_sorted_visibility(c, path)
    header_no_offset_comments(c, path)
    common_this_prefix(c, path)
    common_const_reference(c, path)
    header_lowercase_member_offset_vars(c, path)
    common_self_other(c, path, True)
    common_consistent_float_literals(c, path)

def _check_file_content(content, file_str):
    if file_str.endswith('.h'):
        check_header(content, file_str)
    elif file_str.endswith('.cpp'):
        check_source(content, file_str)
    else:
        FAIL("Must only contain .h and .cpp files!", "NOT APPLICABLE", file_str)

def check_file(file_str):
    st = os.stat(file_str)
    if st.st_mode & stat.S_IXUSR:
        FAIL("Source and header files aren't allowed to be executable!", "NOT APPLICABLE", file_str)

    file = open(file_str, mode="r")
    content = file.read()
    file.close()
    _check_file_content(content, file_str)

def read_csv_file(path):
    if not os.path.isfile(path):
        raise FileNotFoundError('CSV File not found')

    rows = []

    with open(path, "r") as file:
        reader = csv.reader(file)
        for row in reader:
            rows.append(row)

    return rows;

@cache
def get_string_table():
    return read_csv_file(project_root / 'data' / "data_strings.csv");

project_root = setup.ROOT

def main():
    parser = argparse.ArgumentParser(
        'check-format.py', description="Verify additional formatting options next to clang-format and clang-tidy")
    parser.add_argument('-F', '--run-clang-format', action='store_true',
                        help="Automatically run clang format before checking each file")
    parser.add_argument('-a', '--all', action='store_true',
                        help="Run all checks even if one of them fails")
    parser.add_argument('--ci', action='store_true',
                        help="Run in CI mode, meant for github actions and other CI platforms")
    args = parser.parse_args()

    global runAllChecks
    runAllChecks = args.all

    global functionData
    functionData = sorted(utils.get_functions(), key=lambda info: info.name)

    if not args.run_clang_format and not args.ci:
        print("Warning: Input files not being formatted correctly may cause false fails for some checks, to automatically run clang-format use '--run-clang-format' (or '-F')")
        print()

    for dir in [project_root/'lib'/'al', project_root/'src']:
        for root, _, files in os.walk(dir):
            for file in files:
                file_path = os.path.join(root, file)
                file_str = str(file_path)
                if args.run_clang_format:
                    subprocess.check_call(['clang-format', '-i', file_str])
                if args.ci:
                    if subprocess.run(['clang-format', file_str, '--dry-run', '--Werror'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode != 0:
                        print("Warning: file", os.path.relpath(file_str, os.getcwd()), "wasn't formatted correctly with clang-format, this may cause the line numbers to be incorrect")
                        print()
                        result = subprocess.run(['clang-format', file_str], capture_output=True, text=True)
                        if result.returncode == 0:
                            _check_file_content(str(result.stdout), file_str)
                            continue
                check_file(file_str)

    if issueFound:
        exit(1)
    print("No issues found!")

if __name__ == "__main__":
    main()
