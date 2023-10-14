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


def common_newline_eof(c, path):
    CHECK(lambda a:a=="", c.split("\n")[-1], "Files should end with a newline!", path)

# Header files

def header_pragma_once(c, path):
    CHECK(lambda a:a=="#pragma once", c.splitlines()[0], "Headers must start with \"#pragma once\"!", path)


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
    source_include_header(c, path)

def check_header(c, path):
    common_newline_eof(c, path)
    header_pragma_once(c, path)

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
