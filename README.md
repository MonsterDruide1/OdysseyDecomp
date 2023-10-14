# OdysseyDecomp ![Decompiled Status](https://img.shields.io/badge/dynamic/json?url=https://monsterdruide.one/OdysseyDecomp/progress.json&query=$.matching&suffix=%&label=decompiled&color=blue)
Decompilation of all Super Mario Odyssey versions, from 1.0.0 to 1.3.0.

# Building

Reminder: **this will not produce a playable game.** This project will not allow you to play the game if you don't already own it on a Switch.

## For Windows users

While Linux is not a hard requirement, it is strongly advised to [set up WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) to simplify the setup process. Ubuntu 20.04 is usually a good choice.

The instructions below assume that you are using Linux (native or WSL) or macOS.

## 1. Set up dependencies

* Python 3.6 or newer with [pip](https://pip.pypa.io/en/stable/installation/)
* Ninja
* CMake 3.13+
    * If you are on Ubuntu 18.04, you must first [update CMake by using the official CMake APT repository](https://apt.kitware.com/).
* ccache (to speed up builds)
* xdelta3
* clang (not for compiling SMO code, but for compiling Rust tools)

Ubuntu users can install those dependencies by running:

```shell
sudo apt install python3 ninja-build cmake ccache xdelta3 clang libssl-dev libncurses5
```

Additionally, you'll also need:

* A Rust toolchain ([follow the instructions here](https://www.rust-lang.org/tools/install))
* The following Python modules: `capstone colorama cxxfilt pyelftools ansiwrap watchdog python-Levenshtein toml` (install them with `pip install ...`)

## 2. Set up the project

1. Clone this repository. If you are using WSL, please clone the repo *inside* WSL, *not* on the Windows side (for performance reasons).

2. Run `git submodule update --init --recursive`

    Next, you'll need to acquire the **original 1.0 `main` NSO executable**.

    * To dump it from a Switch, follow [the instructions on the wiki](https://zeldamods.org/wiki/Help:Dumping_games#Dumping_binaries_.28executable_files.29).
    * You do not need to dump the entire game (RomFS + ExeFS + DLC). Just dumping the 1.0 ExeFS is sufficient.

3. Run `tools/setup.py [path to the NSO]`
    * This will:
        * install tools/check to check for differences in decompiled code
        * convert the executable if necessary
        * set up [Clang 3.9.1](https://releases.llvm.org/download.html#3.9.1) by downloading it from the official LLVM website
        * set up [Clang 4.0.1](https://releases.llvm.org/download.html#4.0.1) by downloading it from the official LLVM website
        * create a build directory in `build/`
    * If something goes wrong, follow the instructions given to you by the script.
    * If you wish to use a CMake generator that isn't Ninja, use `--cmake_backend` to specify it.

## 3. Build

To start the build, just run

```shell
tools/build.py
```

By default, a multithreaded build is performed. No need to specify `-j` manually.

Use `--clean` to perform a clean build, and `--verbose` to enable verbose output.

To check whether everything built correctly, just run `tools/check` after the build completes.

# Contributing
Anyone is welcome to contribute to this project, just send a pull request!

### TODO
- Enable comparison between different versions and check for mis-matches in all versions using `tools/check`
- 1.3.0 uses a different optimization method, find it and implement it into the toolchain

#### from the re-organization
- Rework the al/Library/Yaml-File structure (should be fewer files, merge a few of them)
- Find proper place for Factories (+Placement of ActorFactory?)
- Graph in Rails misordered
- LiveActorGroup vs. Kit are in the wrong order
- Split files/functions in Library/Resource
- Library/Stage: Proper place for StageInfo
- Library/Math: Split up into multiple files
- Library/Player: Re-organize Util
- game/Util/ResourceUtil remove/cleanup
- Once open-ead/sead#130 is merged, clean up RootTask

# Credits
This decompilation uses [this](https://github.com/open-ead/sead) as a reference for the sead library used. Big thanks to their research!
