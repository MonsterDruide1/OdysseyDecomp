#!/usr/bin/env python3

import argparse
from setup import get_build_dir
import subprocess
import os
import re
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
            if l != "":
                if "::" not in l:
                    allowed_namespaces.append(l)
                else:
                    allowed_namespaces += l.split("::")

        parts = re.split("([{}])", line)
        for x in parts:
            if x == "{":
                nest_level.append("")
                continue
            if x == "}":
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

def common_newline_eof(c, path):
    CHECK(lambda a:a=="", c.split("\n")[-1], "Files should end with a newline!", path)

# Header files

def header_pragma_once(c, path):
    CHECK(lambda a:a=="#pragma once", c.splitlines()[0], "Headers must start with \"#pragma once\"!", path)

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

def source_include_header(c, path):
    # hardcoded exception: Init.cpp contains C functions and no header to relate to
    if(path.endswith("src/System/Init.cpp")): return

    rel_path = path.split("src/")[-1] if "src/" in path else path.split("include/")[-1]
    header_line = "#include \""+rel_path[0:-3]+"h\""
    CHECK(lambda a:a==header_line, c.splitlines()[0], "Source files must start with including respective header in double quotes (here: "+header_line+")!", path)

# -----
# UTILS
# -----

def check_source(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    source_include_header(c, path)

def check_header(c, path):
    common_newline_eof(c, path)
    common_no_namespace_qualifiers(c, path)
    header_pragma_once(c, path)
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
