#!/usr/bin/env python3

import argparse
import hashlib
import platform
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile
import urllib.request

ROOT = Path(__file__).parent.parent


def fail(error: str):
    print(">>> " + error)
    sys.exit(1)


def _get_tool_binary_path():
    base = ROOT / "tools" / "nx-decomp-tools-binaries"
    system = platform.system()
    if system == "Linux":
        return str(base / "linux") + "/"
    if system == "Darwin":
        return str(base / "macos") + "/"
    return ""


def _convert_nso_to_elf(nso_path: Path):
    print(">>>> converting NSO to ELF...")
    binpath = _get_tool_binary_path()
    subprocess.check_call([binpath + "nx2elf", str(nso_path)])


def _decompress_nso(nso_path: Path, dest_path: Path):
    print(">>>> decompressing NSO...")
    binpath = _get_tool_binary_path()
    subprocess.check_call([binpath + "hactool", "-tnso",
                           "--uncompressed=" + str(dest_path), str(nso_path)])


def prepare_executable(original_nso: Path):
    COMPRESSED_V150_HASH = "e21692d90f8fd2def2d2d22d983d62ac81df3b8b3c762d1f2dca9d9ab7b3053a"
    UNCOMPRESSED_V150_HASH = "18ece865061704d551fe456e0600c604c26345ecb38dcbe328a24d5734b3b4eb"

    # The uncompressed v1.5.0 main NSO.
    TARGET_HASH = UNCOMPRESSED_V150_HASH
    TARGET_PATH = ROOT / "data" / "main.nso"
    TARGET_ELF_PATH = ROOT / "data" / "main.elf"

    if TARGET_PATH.is_file() and hashlib.sha256(TARGET_PATH.read_bytes()).hexdigest() == TARGET_HASH and TARGET_ELF_PATH.is_file():
        print(">>> NSO is already set up")
        return

    if not original_nso.is_file():
        fail(f"{original_nso} is not a file")

    nso_data = original_nso.read_bytes()
    nso_hash = hashlib.sha256(nso_data).hexdigest()

    if nso_hash == UNCOMPRESSED_V150_HASH:
        print(">>> found uncompressed 1.5.0 NSO")
        TARGET_PATH.write_bytes(nso_data)

    elif nso_hash == COMPRESSED_V150_HASH:
        print(">>> found compressed 1.5.0 NSO")
        _decompress_nso(original_nso, TARGET_PATH)

    else:
        fail(f"unknown executable: {nso_hash}")

    if not TARGET_PATH.is_file():
        fail("internal error while preparing executable (missing NSO); please report")
    if hashlib.sha256(TARGET_PATH.read_bytes()).hexdigest() != TARGET_HASH:
        fail("internal error while preparing executable (wrong NSO hash); please report")

    _convert_nso_to_elf(TARGET_PATH)

    if not TARGET_ELF_PATH.is_file():
        fail("internal error while preparing executable (missing ELF); please report")


def set_up_compiler(version):
    compiler_dir = ROOT / "toolchain" / ("clang-"+version)
    if compiler_dir.is_dir():
        print(">>> clang is already set up: nothing to do")
        return

    system = platform.system()
    machine = platform.machine()

    if version == "3.9.1":
        builds = {
            # Linux
            ("Linux", "x86_64"): {
                "url": "https://releases.llvm.org/3.9.1/clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz",
                "dir_name": "clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04",
            },
            ("Linux", "aarch64"): {
                "url": "https://releases.llvm.org/3.9.1/clang+llvm-3.9.1-aarch64-linux-gnu.tar.xz",
                "dir_name": "clang+llvm-3.9.1-aarch64-linux-gnu",
            },
        }
    elif version == "4.0.1":
        builds = {
            # Linux
            ("Linux", "x86_64"): {
                "url": "https://releases.llvm.org/4.0.1/clang+llvm-4.0.1-x86_64-linux-gnu-Fedora-25.tar.xz",
                "dir_name": "clang+llvm-4.0.1-x86_64-linux-gnu-Fedora-25",
            },
            ("Linux", "aarch64"): {
                "url": "https://releases.llvm.org/4.0.1/clang+llvm-4.0.1-aarch64-linux-gnu.tar.xz",
                "dir_name": "clang+llvm-4.0.1-aarch64-linux-gnu",
            },
        }

    build_info = builds.get((system, machine))
    if build_info is None:
        fail(
            f"unknown platform: {platform.platform()} (please report if you are on Linux)")

    url: str = build_info["url"]
    dir_name: str = build_info["dir_name"]

    print(f">>> downloading Clang {version} from {url}...")
    with tempfile.TemporaryDirectory() as tmpdir:
        path = tmpdir + "/" + url.split("/")[-1]
        urllib.request.urlretrieve(url, path)

        print(f">>> extracting Clang {version}...")
        with tarfile.open(path) as f:
            f.extractall(compiler_dir.parent)
            (compiler_dir.parent / dir_name).rename(compiler_dir)

    print(">>> successfully set up Clang "+version)


def create_build_dir():
    build_dir = ROOT / "build"
    if build_dir.is_dir():
        print(">>> build directory already exists: nothing to do")
        return

    subprocess.check_call(
        "cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=toolchain/ToolchainNX64.cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -B build/".split(" "))
    print(">>> created build directory")


def main():
    parser = argparse.ArgumentParser(
        "setup.py", description="Set up the Super Mario Odyssey decompilation project")
    parser.add_argument("original_nso", type=Path,
                        help="Path to the original NSO (compressed or not)")
    args = parser.parse_args()

    prepare_executable(args.original_nso)
    set_up_compiler("3.9.1")
    set_up_compiler("4.0.1")
    create_build_dir()


if __name__ == "__main__":
    main()
