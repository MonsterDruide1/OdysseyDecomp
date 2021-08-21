# OdysseyDecomp
Decompilation of all Super Mario Odyssey versions, from 1.0.0 to 1.3.0.

# Building

Reminder: **this will not produce a playable game.** This project will not allow you to play the game if you don't already own it on a Switch.

## For Windows users

While Linux is not a hard requirement, it is strongly advised to [set up WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) to simplify the setup process. Ubuntu 20.04 is usually a good choice.

The instructions below assume that you are using Linux (native or WSL) or macOS.

## 1. Set up dependencies

* Python 3.6 or newer
* Ninja
* CMake 3.13+
    * If you are on Ubuntu 18.04, you must first [update CMake by using the official CMake APT repository](https://apt.kitware.com/).
* ccache (to speed up builds)

Ubuntu users can install those dependencies by running:

```shell
sudo apt install python3 ninja-build cmake ccache
```

Additionally, you'll also need:

* A Rust toolchain ([follow the instructions here](https://www.rust-lang.org/tools/install))

## 2. Set up the project

1. Clone this repository. If you are using WSL, please clone the repo *inside* WSL, *not* on the Windows side (for performance reasons).

2. Run `git submodule update --init --recursive`

    Next, you'll need to acquire the **original 1.0 `main` NSO executable**.

    * To dump it from a Switch, follow [the instructions on the wiki](https://zeldamods.org/wiki/Help:Dumping_games#Dumping_binaries_.28executable_files.29).
    * You do not need to dump the entire game (RomFS + ExeFS + DLC). Just dumping the 1.0 ExeFS is sufficient.

3. Download Clang 3.9.1 manually
    * As we're not sure about the right compiler version yet, it won't be downloaded automatically. 
    * Download a prebuilt binary (version 3.9.1) from [LLVM](https://releases.llvm.org/download.html#3.9.1), matching your environment.
    * Extract it to `toolchain/clang-3.9.1`, so that the file `toolchain/clang-3.9.1/bin/clang` exists.

4. Run `tools/common/setup.py [path to the NSO]`
    * This will:
        * install tools/check to check for differences in decompiled code
        * convert the executable if necessary
        * set up [Clang 4.0.1](https://releases.llvm.org/download.html#4.0.1) by downloading it from the official LLVM website
        * create a build directory in `build/`
    * If something goes wrong, follow the instructions given to you by the script.

## 3. Build

To start the build, just run

```shell
ninja -C build
```

By default, Ninja will perform a multithreaded build. There is no need to pass -j manually.

To check whether everything built correctly, just run `tools/check` after the build completes.

# Contributing
Anyone is welcome to contribute to this project, just send a pull request!

### TODO
- Enable comparison between different versions and check for mis-matches in all versions using `tools/check`
- 1.3.0 uses a different optimization method, find it and implement it into the toolchain

# Credits
This decompilation uses [this](https://github.com/open-ead/sead) as a reference for the sead library used. Big thanks to their research!