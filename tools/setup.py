#!/usr/bin/env python3

import argparse
import hashlib
from pathlib import Path
import subprocess
from typing import Optional
from common import setup_common as setup
from enum import Enum

class Version(Enum):
    VER_100 = "1.0"
    VER_101 = "1.0.1"
    VER_110 = "1.1"
    VER_120 = "1.2"
    VER_130 = "1.3"

def prepare_executable(original_nso: Optional[Path]):
    COMPRESSED_V10_HASH = "e21692d90f8fd2def2d2d22d983d62ac81df3b8b3c762d1f2dca9d9ab7b3053a"
    UNCOMPRESSED_V10_HASH = "18ece865061704d551fe456e0600c604c26345ecb38dcbe328a24d5734b3b4eb"

    # The uncompressed v1.0 main NSO.
    TARGET_HASH = UNCOMPRESSED_V10_HASH
    TARGET_PATH = setup.ROOT / "data" / "main.nso"
    TARGET_ELF_PATH = setup.ROOT / "data" / "main.elf"

    if TARGET_PATH.is_file() and hashlib.sha256(TARGET_PATH.read_bytes()).hexdigest() == TARGET_HASH and TARGET_ELF_PATH.is_file():
        print(">>> NSO is already set up")
        return

    if not original_nso.is_file():
        setup.fail(f"{original_nso} is not a file")

    nso_data = original_nso.read_bytes()
    nso_hash = hashlib.sha256(nso_data).hexdigest()

    if nso_hash == UNCOMPRESSED_V10_HASH:
        print(">>> found uncompressed 1.0 NSO")
        TARGET_PATH.write_bytes(nso_data)

    elif nso_hash == COMPRESSED_V10_HASH:
        print(">>> found compressed 1.0 NSO")
        setup._decompress_nso(original_nso, TARGET_PATH)

    else:
        setup.fail(f"unknown executable: {nso_hash}")

    if not TARGET_PATH.is_file():
        setup.fail("internal error while preparing executable (missing NSO); please report")
    if hashlib.sha256(TARGET_PATH.read_bytes()).hexdigest() != TARGET_HASH:
        setup.fail("internal error while preparing executable (wrong NSO hash); please report")

    setup._convert_nso_to_elf(TARGET_PATH)

    if not TARGET_ELF_PATH.is_file():
        setup.fail("internal error while preparing executable (missing ELF); please report")

def create_build_dir(ver):
    if(ver != Version.VER_100): return # TODO remove this when multiple versions should be built
    build_dir = setup.ROOT / "build" # ("build-"+ver.value)
    if build_dir.is_dir():
        print(">>> build directory already exists: nothing to do")
        return

    subprocess.check_call(
        ("cmake -GNinja -DCMAKE_CXX_FLAGS=-D"+ver.name+" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=toolchain/ToolchainNX64.cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -B "+str(build_dir)).split(" "))
    print(">>> created build directory") 

def main():
    parser = argparse.ArgumentParser(
        "setup.py", description="Set up the Super Mario Odyssey decompilation project")
    parser.add_argument("original_nso", type=Path,
                        help="Path to the original NSO (1.0, compressed or not)", nargs="?")
    args = parser.parse_args()

    setup.install_viking()
    prepare_executable(args.original_nso)
    setup.set_up_compiler("4.0.1")
    print("Please download and extract clang-3.9.1 manually, until it has been properly added to the setup chain.")
    create_build_dir(Version.VER_100)
    create_build_dir(Version.VER_101)
    create_build_dir(Version.VER_110)
    create_build_dir(Version.VER_120)
    create_build_dir(Version.VER_130)


if __name__ == "__main__":
    main()
