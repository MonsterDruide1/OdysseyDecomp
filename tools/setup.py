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
CACHE_REPO_RELEASE_URL = "https://github.com/MonsterDruide1/OdysseyDecompToolsCache/releases/download/v1.2.3"
TARGET_UNCOMPRESSED_NSO_PATH = setup.config.get_versioned_data_path(setup.config.get_default_version()) / 'main.uncompressed.nso'
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
    V10_ELF_HASH = "b8f8b542c1ee6bd3eb70c9ccebb52b69c9f7ced5f7cd8aebed56ec8fe53b3aa5"

    if TARGET_ELF_PATH.is_file() and hashlib.sha256(TARGET_ELF_PATH.read_bytes()).hexdigest() == V10_ELF_HASH:
        print(">>> Converted ELF is already set up")
        return

    if not original_nso.is_file():
        setup.fail(f"{original_nso} is not a file")

    nso_hash = hashlib.sha256(original_nso.read_bytes()).hexdigest()

    if nso_hash != COMPRESSED_V10_HASH and nso_hash != UNCOMPRESSED_V10_HASH:
        setup.fail(f"unknown executable: {nso_hash}")

    setup._convert_nso_to_elf(original_nso)

    converted_elf_path = original_nso.with_suffix(".elf")

    if not converted_elf_path.is_file():
        setup.fail("internal error while preparing executable (missing ELF) please report")

    shutil.move(converted_elf_path, TARGET_ELF_PATH)

    uncompressed_nso_path = original_nso.with_suffix(".uncompressed.nso")
    shutil.move(uncompressed_nso_path, TARGET_UNCOMPRESSED_NSO_PATH)

    if not TARGET_UNCOMPRESSED_NSO_PATH.is_file() or hashlib.sha256(TARGET_UNCOMPRESSED_NSO_PATH.read_bytes()).hexdigest() != UNCOMPRESSED_V10_HASH:
        setup.fail("Internal error while exporting uncompressed NSO (uncompressed NSO either doesn't exist or has an incorrect hash) please report")

def check_download_url_updated():
    if not exists_toolchain_file("cache-version-url.txt"):
        return True
    with open(f"{get_repo_root()}/toolchain/cache-version-url.txt", "r+") as f:
        data = f.read()
        if data != CACHE_REPO_RELEASE_URL:
            return True
    return False


def get_build_dir():
    return setup.ROOT / "build"

def exists_toolchain_file(file_path_rel):
    return os.path.isfile(f"{get_repo_root()}/toolchain/{file_path_rel}")

def setup_project_tools(tools_from_source):

    def exists_tool(tool_name, check_symlink=True):
        return os.path.isfile(f"{get_repo_root()}/tools/{tool_name}") or (check_symlink and os.path.islink(f"{get_repo_root()}/tools/{tool_name}"))

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

    def update_current_cache_url():
        with open(f"{get_repo_root()}/toolchain/cache-version-url.txt", "w") as f:
            f.write(CACHE_REPO_RELEASE_URL)

    def remove_old_toolchain():
        if exists_toolchain_file("clang-3.9.1/bin/clang"):
            print("Removing toolchain/clang-3.9.1 since full toolchains are no longer needed")
            shutil.rmtree(f"{get_repo_root()}/toolchain/clang-3.9.1")
        if exists_toolchain_file("clang-4.0.1/bin/lld"):
            print("Removing toolchain/clang-4.0.1")
            shutil.rmtree(f"{get_repo_root()}/toolchain/clang-4.0.1")

    remove_old_toolchain()
    if check_download_url_updated():
        print("Old toolchain files found. Replacing them with ones from the latest release")
        if exists_tool("check", False):
            os.remove(f"{get_repo_root()}/tools/check")
        if exists_tool("decompme", False):
            os.remove(f"{get_repo_root()}/tools/decompme")
        if exists_tool("listsym", False):
            os.remove(f"{get_repo_root()}/tools/listsym")
        if exists_toolchain_file("bin/clang"):
            shutil.rmtree(f"{get_repo_root()}/toolchain/bin")

    if not exists_tool("check"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/check", f"{get_repo_root()}/tools/check")
    if not exists_tool("decompme"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/decompme", f"{get_repo_root()}/tools/decompme")
    if not exists_tool("listsym"):
        os.symlink(f"{get_repo_root()}/toolchain/bin/listsym", f"{get_repo_root()}/tools/listsym")

    with tempfile.TemporaryDirectory() as tmpdir:
        if exists_toolchain_file("include/__config"):  # old path to libcxx headers => moved to `libcxx-include`
            print("Removing old libc++ headers...")
            shutil.rmtree(f"{get_repo_root()}/toolchain/include")

        if not exists_toolchain_file("libcxx-include/__config"):
            print(">>> Downloading llvm-3.9 libc++ headers...")
            path = tmpdir + "/libcxx-3.9.1.src.tar.xz"
            urllib.request.urlretrieve(LIBCXX_SRC_URL, path)
            print(">>> Extracting libc++ headers...")
            with tarfile.open(path) as f:
                f.extractall(tmpdir, filter='tar')
            shutil.copytree(f"{tmpdir}/libcxx-3.9.1.src/include", f"{get_repo_root()}/toolchain/libcxx-include", dirs_exist_ok=True)

        if not exists_tool("check") or not exists_tool("decompme") or not exists_tool("listsym") or not exists_toolchain_file("bin/clang") or not exists_toolchain_file("bin/ld.lld"):

            if os.path.isdir(get_build_dir()):
                shutil.rmtree(get_build_dir())

            if tools_from_source:
                build_tools_from_source(tmpdir)
                update_current_cache_url()
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
            update_current_cache_url()

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

    if "IN_NIX_SHELL" in os.environ and "SMO_NIX_SETUP" not in os.environ:
        print("nix users must run `nix run .#setup -- [path to NSO]` instead.")
        exit(1)

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
