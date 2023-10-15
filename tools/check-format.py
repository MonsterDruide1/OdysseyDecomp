#!/usr/bin/env python3

import argparse
from setup import get_build_dir
import subprocess
import os
import re
from functools import cache
from common import setup_common as setup

# ------
# CHECKS
# ------

issueFound = False

def FAIL(message, line, path):
    print("Offending file:", path)
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
        line = line[0:line.find("//")] if "//" in line else line
        if line.startswith("using namespace"):
            match = re.search("^using namespace ([^;\s]+);$", line)
            if CHECK(lambda a:match, line, "Unexpected \"using namespace\" line: should follow format \"using namespace xy;\"", path): return
            continue
        if CHECK(lambda a:a.rfind("namespace") in [-1, 0], line, "\"namespace\" must only be listed at the start of a line!", path): return

        if line.startswith("namespace"):
            match = re.search("^namespace ([^{\s]*) ?{$", line)
            if CHECK(lambda a:match, line, "Unexpected namespace line: should follow format \"namespace xy {\"", path): return
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
                    if CHECK(lambda a:a=="}", line.rstrip(), "Closing namespace expects only \"}\" in line!", path): return
                del nest_level[-1]
                continue

            matches = re.findall("[\(,\s]([^\(,\s]+::)+[^\(,\s]+", x)
            for match in matches:
                match = match[0:-2]
                # examples: "sead", "al", "nn::g3d"
                if CHECK(lambda a:match not in allowed_namespaces, line, match+" should be omitted here!", path): return

    if len(nest_level) != 0:
        print("ERROR: nest_level not empty at end of the file!")
        print("nest_level", nest_level)
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
    cpp_files = ["concepts","coroutine","any","bitset","chrono","compare","csetjmp","csignal","cstdarg","cstddef","cstdlib","ctime","expected","functional","initializer_list","optional","source_location","tuple","type_traits","typeindex","typeinfo","utility","variant","version","memory","memory_resource","new","scoped_allocator","cfloat","cinttypes","climits","cstdint","limits","stdfloat","cassert","cerrno","exception","stacktrace","stdexcept","system_error","cctype","charconv","cstring","cuchar","cwchar","cwctype","format","string","string_view","array","deque","flat_map","flat_set","forward_list","list","map","mdspan","queue","set","span","stack","unordered_map","unordered_set","vector","iterator","generator","ranges","algorithm","execution","bit","cfenv","cmath","complex","numbers","numeric","random","ratio","valarray","clocale","codecvt","locale","text_encoding","cstdio","fstream","iomanip","ios","iosfwd","iostream","istream","ostream","print","spanstream","sstream","streambuf","strstream","syncstream","filesystem","regex","atomic","barrier","condition_variable","future","hazard_pointer","latch","mutex","rcu","semaphore","shared_mutex","stop_token","thread","stdatomic.h"]
    sead_files = get_files(project_root/'lib'/'sead'/'include')
    nintendo_sdk_files = get_files(project_root/'lib'/'NintendoSDK'/'include')
    agl_files = get_files(project_root/'lib'/'agl'/'include')
    aarch_files = get_files(project_root/'lib'/'aarch64')
    eui_files = get_files(project_root/'lib'/'eui'/'include')
    al_files = get_files(project_root/'lib'/'al'/'include')
    game_files = [a for a in get_files(project_root/'src') if a.endswith(".h")]

    angled_includes = cpp_files + aarch_files + nintendo_sdk_files + sead_files + agl_files + eui_files
    al_includes = al_files
    game_includes = game_files

    return angled_includes, al_includes, game_includes

def common_include_order(c, path, is_header):
    lines = c.split("\n")
    if is_header:
        if CHECK(lambda a:a=="#pragma once", lines[0], "Headers must start with \"#pragma once\"!", path): return
        del lines[0]
        if CHECK(lambda a:a=="", lines[0], "\"#pragma once\" must be followed by exactly one empty line!", path): return
        del lines[0]
    elif not path.endswith("src/System/Init.cpp"):
        # hardcoded exception: Init.cpp contains C functions and no header to relate to
        rel_path = path.split("src/")[-1] if "src/" in path else path.split("include/")[-1]
        header_line = "#include \""+rel_path[0:-3]+"h\""
        if CHECK(lambda a:a==header_line, lines[0], "Source files must start with including respective header in double quotes (here: "+header_line+")!", path): return
        del lines[0]
        if CHECK(lambda a:a=="", lines[0], "\""+header_line+"\" must be followed by exactly one empty line!", path): return
        del lines[0]

    include_lines = []
    end_of_includes = False
    for line in lines:
        if line.startswith("#include"):
            if CHECK(lambda a:not end_of_includes, line, "Includes have to be listed at the very top of the file!", path): return
            include_lines.append(line)
        elif line == "" and not end_of_includes:
            include_lines.append(line)
        else:
            end_of_includes = True

    angled_includes, al_includes, game_includes = get_includes()

    order = -1  # -1=none (after initial newline) ; 0=angled (sead, agl, nn, eui) ; 1=al ; 2=game ; -2,-3,-4=newline after respective section (angled, al, game)
    for line in include_lines:
        if line=="":
            if CHECK(lambda a:order in [0,1,2], line, "Unexpected newline in includes! Please re-read the contribution guide and format accordingly.", path): return
            order = -order-2
            continue

        if line.startswith("#include <") and line.endswith(">"):
            file = line[10:-1]
            if CHECK(lambda a:order in [-1,0], line, "Angled \"#includes <...>\" can only appear in the first block of includes!", path): return
            if CHECK(lambda a:file in angled_includes, line, "This file is not allowed to be included with <>!", path): return
            order = 0
        elif line.startswith("#include \"") and line.endswith("\""):
            file = line[10:-1]
            if file in al_includes:
                if CHECK(lambda a:order in [-1,-2,1], line, "Wrong order for includes: Found \"al\"-include outside of expected block!", path): return
                order=1
            elif file in game_includes:
                if CHECK(lambda a:order in [-1,-2,-3,2], line, "Wrong order for includes: Found \"game\"-include outside of expected block!", path): return
                order=2
            else:
                FAIL("This file is not allowed to be included with <>!", line, path)
                return
        else:
            FAIL("Unknown include format", line, path)
            return
    
    CHECK(lambda a:order in [-1,-2,-3,-4], "not applicable", "Empty line expected after includes!", path)

def common_newline_eof(c, path):
    CHECK(lambda a:a=="", c.split("\n")[-1], "Files should end with a newline!", path)

# Header files

def header_sorted_visibility(c, path):
    visibilities_ordered = ["public:", "protected:", "private:"]
    nest_level = [-1]  # outermost for macro definitions
    for line in c.splitlines():
        line = line[0:line.find("//")] if "//" in line else line
        if line.endswith("\\"): line = line[0:-1].rstrip()
        if "{" in line and "}" in line:
            if CHECK(lambda a:a.count("{")==a.count("}") or (a.lstrip().startswith("{") and a.endswith("}};")), line, "Unbalanced \"{\" and \"}\" in the same line! (exception: end of brace-initialized array)", path): return
            if line.lstrip().startswith("{") and line.endswith("}};"):
                del nest_level[-1]
            continue
        
        if CHECK(lambda a:[b for b in visibilities_ordered if b in a and a!=b]==[], line.strip(), "visibility modificator must be its own line!", path): return
        if CHECK(lambda a:a.count("{")+a.count("}")<=1, line, "Only one \"{\" and \"}\" is allowed per line!", path): return

        if line in visibilities_ordered:
            i = visibilities_ordered.index(line)
            if CHECK(lambda a:i>nest_level[-1], line, "Wrong order of visibilities: Must be public, protected, private!", path): return
            nest_level[-1] = i
            continue
        elif "{" in line:
            nest_level.append(-1)
        elif "}" in line:
            del nest_level[-1]

    if len(nest_level) != 1:
        print("ERROR: nest_level not empty at end of the file!")
        print("nest_level", nest_level)
        exit(1)
 


# Source files

# -----
# UTILS
# -----

def check_source(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    common_include_order(c, path, False)

def check_header(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    common_include_order(c, path, True)
    header_sorted_visibility(c, path)

def check_file(file_str):
    file = open(file_str, mode="r")
    content = file.read()
    file.close()
    
    if file_str.endswith('.h'):
        check_header(content, file_str)
    elif file_str.endswith('.cpp'):
        check_source(content, file_str)
    else:
        FAIL("Must only contain .h and .cpp files!", "NOT APPLICABLE", file_str)


project_root = setup.ROOT

def main():
    parser = argparse.ArgumentParser(
        'check-format.py', description="Verify additional formatting options next to clang-format and clang-tidy")
    parser.add_argument('--verbose', action='store_true',
                        help="Give verbose output")
    args = parser.parse_args()

    for dir in [project_root/'lib'/'al', project_root/'src']:
        for root, _, files in os.walk(dir):
            for file in files:
                file_path = os.path.join(root, file)
                file_str = str(file_path)
                check_file(file_str)

    if issueFound:
        exit(1)
    else:
        print("No issues found!")

if __name__ == "__main__":
    main()
