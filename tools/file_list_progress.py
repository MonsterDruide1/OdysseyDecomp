#!/usr/bin/env python3

import argparse
from collections import defaultdict
from colorama import Fore
from common.util import utils
from common.util.utils import FunctionStatus
from common.util.config import CONFIG
import typing as tp

parser = argparse.ArgumentParser()
parser.add_argument("--csv", "-c", action="store_true",
                    help="Print a CSV line rather than a human-readable progress report")
parser.add_argument("--print-nm", "-n", action="store_true",
                    help="Print non-matching functions with major issues")
parser.add_argument("--print-eq", "-e", action="store_true",
                    help="Print non-matching functions with minor issues")
parser.add_argument("--print-ok", "-m", action="store_true",
                    help="Print matching functions")
parser.add_argument("--version",
                    help="Specify which version to load CSV from")
args = parser.parse_args()

code_size_total = 0
num_total = 0
code_size: tp.DefaultDict[FunctionStatus, int] = defaultdict(int)
counts: tp.DefaultDict[FunctionStatus, int] = defaultdict(int)

# TODO: Move these to uitls.py once the new file list format is upstreamed

class FileListEntryInfo:
    def __init__(self, label: str, status: FunctionStatus, size: int):
        self.label = label
        self.status = status
        self.size = size

_status_map = {
    "Matching": FunctionStatus.Matching,
    "NonMatchingMajor": FunctionStatus.NonMatching,
    "NonMatchingMinor": FunctionStatus.Equivalent,
    "NotDecompiled": FunctionStatus.NotDecompiled,
    "Wip": FunctionStatus.Wip,
}

# Manual parsing for better performance
def parse_file_list_data() -> tp.List[FileListEntryInfo]:
    funtions = []
    with open(CONFIG["file_list"]) as file_list:
        current_size = 0
        current_label = ""
        file_list_lines = list(file_list)
        for i, line in enumerate(file_list_lines):
            line = line.strip()
            if "size:" in line:
                line_parts = line.split(" ")
                current_size = int(line_parts[-1])
            elif "label:" in line:
                # Get last element if label is a string array
                if "-" in file_list_lines[i + 1]:
                    current_label = file_list_lines[i + 1].split(" ")[-1]
                else:
                    current_label = line.split(" ")[-1]
            elif "status:" in line:
                line_parts = line.split(" ")
                status = _status_map.get(line_parts[-1], FunctionStatus.NotDecompiled)
                funtions.append(FileListEntryInfo(current_label, status, current_size))
    return funtions

for info in parse_file_list_data():
    code_size_total += info.size
    num_total += 1

    counts[info.status] += 1
    code_size[info.status] += info.size

    if not args.csv:
        if info.status == FunctionStatus.NonMatching:
            if args.print_nm:
                print(f"{Fore.RED}NM{Fore.RESET} {utils.format_symbol_name(info.label)}")
        elif info.status == FunctionStatus.Equivalent:
            if args.print_eq:
                print(f"{Fore.YELLOW}EQ{Fore.RESET} {utils.format_symbol_name(info.label)}")
        elif info.status == FunctionStatus.Matching:
            if args.print_ok:
                print(f"{Fore.GREEN}OK{Fore.RESET} {utils.format_symbol_name(info.label)}")


def format_progress(label: str, num: int, size: int):
    percentage = round(100 * num / num_total, 3)
    size_percentage = round(100 * size / code_size_total, 3)
    return f"{num:>7d} {label}{Fore.RESET} ({percentage}% | size: {size_percentage}%)"


def format_progress_for_status(label: str, status: FunctionStatus):
    return format_progress(label, counts[status], code_size[status])


if args.csv:
    import git

    version = 1
    git_object = git.Repo().head.object
    timestamp = str(git_object.committed_date)
    git_hash = git_object.hexsha

    fields = [
        str(version),
        timestamp,
        git_hash,

        str(num_total),
        str(code_size_total),

        str(counts[FunctionStatus.Matching]),
        str(code_size[FunctionStatus.Matching]),

        str(counts[FunctionStatus.Equivalent]),
        str(code_size[FunctionStatus.Equivalent]),

        str(counts[FunctionStatus.NonMatching]),
        str(code_size[FunctionStatus.NonMatching]),
    ]
    print(",".join(fields))

else:
    print()

    print(f"{num_total:>7d} functions (size: ~{code_size_total} bytes)")

    count_decompiled = counts[FunctionStatus.Matching] + counts[FunctionStatus.Equivalent] + counts[
        FunctionStatus.NonMatching]
    code_size_decompiled = code_size[FunctionStatus.Matching] + code_size[FunctionStatus.Equivalent] + code_size[
        FunctionStatus.NonMatching]

    print(format_progress(f"{Fore.CYAN}decompiled", count_decompiled, code_size_decompiled))
    print(format_progress_for_status(f"{Fore.GREEN}matching", FunctionStatus.Matching))
    print(format_progress_for_status(f"{Fore.YELLOW}non-matching (minor issues)", FunctionStatus.Equivalent))
    print(format_progress_for_status(f"{Fore.RED}non-matching (major issues)", FunctionStatus.NonMatching))
    print()
