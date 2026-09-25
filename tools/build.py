#!/usr/bin/env -S uv run

import argparse
from setup import check_download_url_updated
import subprocess
import os

from nx_decomp_tools.util import config

def warn_outdated_tools():
    if check_download_url_updated():
        from colorama import Fore
        print(f"{Fore.YELLOW}Found unexpected version of tools, consider rerunning setup.py to fetch the version matching the current repository. Rebase onto latest master to work with the newest tools.{Fore.RESET}")

def main():
    parser = argparse.ArgumentParser(
        'build.py', description="Build the Super Mario Odyssey decompilation project")
    parser.add_argument('--clean', action='store_true',
                        help="Clean before building")
    parser.add_argument('--verbose', action='store_true',
                        help="Give verbose output")
    args = parser.parse_args()

    build_dir = config.get_build_root()

    if not build_dir.is_dir():
        print("Please run setup.py first.")
        exit(1)

    warn_outdated_tools()

    cmake_args = ['cmake', '--build', str(build_dir)]
    if args.clean:
        cmake_args.append('--clean-first')
    if args.verbose:
        os.environ['VERBOSE'] = '1'

    try:
        subprocess.run(cmake_args, check=True)
    except subprocess.CalledProcessError:
        exit(1)  # silently exit with failure if build failed

if __name__ == "__main__":
    main()
