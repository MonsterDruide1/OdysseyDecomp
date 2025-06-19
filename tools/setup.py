#!/usr/bin/env python3

import argparse
import hashlib
import os
import shutil
from pathlib import Path
import subprocess
from typing import Optional
from common import setup_common as setup
from enum import Enum
import platform
import tarfile
import tempfile
import urllib.request
import urllib.parse
import urllib.error
from common.util.config import get_repo_root

TARGET_PATH = setup.get_target_path()
TARGET_ELF_PATH = setup.get_target_elf_path()
CACHE_REPO_RELEASE_URL = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.0"
LIBCXX_SRC_URL = "https://releases.llvm.org/3.9.1/libcxx-3.9.1.src.tar.xz"

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

def get_build_dir():
    return setup.ROOT / "build"

def setup_project_tools(tools_from_source):

    def exists_tool(tool_name, check_symlink=True):
        return os.path.isfile(f"{get_repo_root()}/tools/{tool_name}") or (check_symlink and os.path.islink(f"{get_repo_root()}/tools/{tool_name}"))

    def exists_toolchain_file(file_path_rel):
        return os.path.isfile(f"{get_repo_root()}/toolchain/{file_path_rel}")

    def build_tools_from_source(tmpdir_path):
        cwd = os.getcwd()
        url_parts = CACHE_REPO_RELEASE_URL.split("/")
        tag_name = url_parts[len(url_parts) - 1]
        subprocess.check_call(["git", "clone", "--depth=1", "--branch", tag_name, "https://github.com/MonsterDruide1/OdysseyDecompToolsCache.git", f"{tmpdir_path}/OdysseyDecompToolsCache"])
        os.chdir(f"{tmpdir}/OdysseyDecompToolsCache")
        subprocess.check_call(["git", "submodule", "update", "--init"])
        subprocess.check_call(["./generate.sh", "--no-tarball"])
        os.chdir(cwd)
        shutil.copytree(f"{tmpdir_path}/OdysseyDecompToolsCache/build/OdysseyDecomp-binaries_{platform.machine()}-{platform.system()}/bin", f"{get_repo_root()}/toolchain/bin")

    def remove_old_toolchain():
        if exists_toolchain_file("clang-3.9.1/bin/clang"):
            print("Removing toolchain/clang-3.9.1 since full toolchains are no longer needed")
            shutil.rmtree(f"{get_repo_root()}/toolchain/clang-3.9.1")
        if exists_toolchain_file("clang-4.0.1/bin/lld"):
            print("Removing toolchain/clang-4.0.1")
            shutil.rmtree(f"{get_repo_root()}/toolchain/clang-4.0.1")

    def check_download_url_updated():
        if not exists_toolchain_file("cache-version-url.txt"):
            with open(f"{get_repo_root()}/toolchain/cache-version-url.txt", "w") as f:
                f.write(CACHE_REPO_RELEASE_URL)
            return
        with open(f"{get_repo_root()}/toolchain/cache-version-url.txt", "r+") as f:
            data = f.read()
            if data != CACHE_REPO_RELEASE_URL:
                f.seek(0)
                f.write(CACHE_REPO_RELEASE_URL)
                f.truncate()
                print("Old toolchain files found. Replacing them with ones from the latest release")
                if exists_tool("check", False):
                    os.remove(f"{get_repo_root()}/tools/check")
                if exists_tool("decompme", False):
                    os.remove(f"{get_repo_root()}/tools/decompme")
                if exists_tool("listsym", False):
                    os.remove(f"{get_repo_root()}/tools/listsym")
                if exists_toolchain_file("bin/clang"):
                    shutil.rmtree(f"{get_repo_root()}/toolchain/bin")


    remove_old_toolchain()
    check_download_url_updated()

    if not exists_tool("check"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/check", f"{get_repo_root()}/tools/check")
    if not exists_tool("decompme"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/decompme", f"{get_repo_root()}/tools/decompme")
    if not exists_tool("listsym"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/listsym", f"{get_repo_root()}/tools/listsym")

    with tempfile.TemporaryDirectory() as tmpdir:
        if not exists_toolchain_file("include/__config"):
            print(">>> Downloading llvm-3.9 libc++ headers...")
            path = tmpdir + "/libcxx-3.9.1.src.tar.xz"
            urllib.request.urlretrieve(LIBCXX_SRC_URL, path)
            print(">>> Extracting libc++ headers...")
            with tarfile.open(path) as f:
                f.extractall(tmpdir, filter='tar')
            shutil.copytree(f"{tmpdir}/libcxx-3.9.1.src/include", f"{get_repo_root()}/toolchain/include", dirs_exist_ok=True)

        if not exists_tool("check") or not exists_tool("decompme") or not exists_tool("listsym") or not exists_toolchain_file("bin/clang") or not exists_toolchain_file("bin/ld.lld"):

            if os.path.isdir(get_build_dir()):
                shutil.rmtree(get_build_dir())

            if tools_from_source:
                build_tools_from_source(tmpdir)
                return

            target = f"{platform.machine()}-{platform.system()}"
            path = tmpdir + f"/OdysseyDecomp-binaries_{target}"
            try:
                print(">>> Downloading clang, lld and viking...")
                url = CACHE_REPO_RELEASE_URL + urllib.parse.quote(f"/OdysseyDecomp-binaries_{target}.tar.xz")
                urllib.request.urlretrieve(url, path)
                print(">>> Extracting tools...")
                with tarfile.open(path) as f:
                    f.extractall(f"{get_repo_root()}/toolchain/", filter='tar')
            except urllib.error.HTTPError:
                input(f"Prebuilt binaries not found for platform: {target}. Do you want to build llvm, clang, lld and viking from source? (Press enter to accept)")
                build_tools_from_source(tmpdir)

def create_build_dir(ver, cmake_backend):
    if(ver != Version.VER_100): return # TODO: remove this when multiple versions should be built
    build_dir = get_build_dir()
    if build_dir.is_dir():
        print(">>> build directory already exists: nothing to do")
        return

    subprocess.check_call(
        ['cmake', '-G', cmake_backend, f'-DCMAKE_CXX_FLAGS=-D{ver.name}', '-DCMAKE_BUILD_TYPE=RelWithDebInfo', '-DCMAKE_TOOLCHAIN_FILE=toolchain/ToolchainNX64.cmake', '-DCMAKE_CXX_COMPILER_LAUNCHER=ccache', '-B', str(build_dir)])
    print(">>> created build directory") 

def main():
    parser = argparse.ArgumentParser(
        "setup.py", description="Set up the Super Mario Odyssey decompilation project")
    parser.add_argument("original_nso", type=Path,
                        help="Path to the original NSO (1.0, compressed or not)", nargs="?")
    parser.add_argument("--cmake_backend", type=str,
                        help="CMake backend to use (Ninja, Unix Makefiles, etc.)", nargs="?", default="Ninja")
    parser.add_argument("--project-only", action="store_true",
                    help="Disable original NSO setup")
    parser.add_argument("--tools-from-src", action="store_true",
                    help="Build llvm, clang, lld and viking from source instead of using a prebuilt binaries")
    args = parser.parse_args()

    setup_project_tools(args.tools_from_src)
    if not args.project_only:
        prepare_executable(args.original_nso)
    create_build_dir(Version.VER_100, args.cmake_backend)
    create_build_dir(Version.VER_101, args.cmake_backend)
    create_build_dir(Version.VER_110, args.cmake_backend)
    create_build_dir(Version.VER_120, args.cmake_backend)
    create_build_dir(Version.VER_130, args.cmake_backend)


if __name__ == "__main__":
    main()
