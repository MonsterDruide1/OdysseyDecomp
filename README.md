# OdysseyDecomp ![Decompiled Status](https://img.shields.io/badge/dynamic/json?url=https://monsterdruide.one/OdysseyDecomp/progress.json&query=$.matching&suffix=%&label=decompiled&color=blue)

Decompilation of all Super Mario Odyssey versions, from 1.0.0 to 1.3.0.

# Building

> [!IMPORTANT]
> Reminder: **this will not produce a playable game.** This project will not allow you to play the game if you don't
> already own it on a Switch.

## For Windows users

While Linux is not a hard requirement, it is strongly advised
to [set up WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) to simplify the setup process. Ubuntu 22.04
is usually a good choice.

The instructions below assume that you are using Linux (native or WSL) or macOS.

## 1. Set up dependencies

Depending on your system and preferences, the required programs and libraries can be setup differently.

Across all platforms, using the included Visual Studio Code Dev Container should work fine. In this case, just clone and
open the folder in VS-Code, press F1 and select `Dev Containers: Rebuild and Reopen in Container`. You can copy the NSO
executable into the container using `docker cp /path/to/main.nso [container-id]:/workspaces/main.nso`, where
`[container-id]` is the ID listed in `docker container ls`.

> [!WARNING]
> As Dev Containers add another layer of complexity to your system, compiling the project and working with the differ
> will be slower. If possible, prefer to install the dependencies on your system and use this project natively instead of
> through the container.

When working with NixOS or any other system with the Nix package manager, `flake.nix` should be properly set up to use
on your system. Make sure that `direnv` is installed in your system or shell. Then create a file called `.envrc.private`
with the following content:

```bash
export USE_NIX=true
```

Finally, run `direnv allow` to setup all dependencies. The remainder of this section can be skipped.

All other systems have to manually install the required packages and programs. We will need:

* Python 3.6 or newer with [pip](https://pip.pypa.io/en/stable/installation/)
* Ninja
* CMake 3.13+
    * If you are on Ubuntu 18.04, you must
      first [update CMake by using the official CMake APT repository](https://apt.kitware.com/).
* ccache (to speed up builds)

Ubuntu users can install those dependencies by running:

```shell
sudo apt install python3 ninja-build cmake ccache libssl-dev libncurses5
```

If you are running Ubuntu 23.10 or later, the `libncurses5` package won't be available anymore. You can install it from
the archive using:

```shell
wget http://archive.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.3-2_amd64.deb && sudo dpkg -i libtinfo5_6.3-2_amd64.deb && rm -f libtinfo5_6.3-2_amd64.deb
```

For other systems, please check for the corresponding `libncurses5` backport, for
example [ncurses5-compat-libs](https://aur.archlinux.org/packages/ncurses5-compat-libs) for Arch-based distributions.

Additionally, you'll also need:

* A Rust toolchain ([follow the instructions here](https://www.rust-lang.org/tools/install))
* The following Python modules: `capstone colorama cxxfilt pyelftools watchdog python-Levenshtein toml` (
  install them with `pip install ...`)

## 2. Set up the project

1. Clone this repository. If you are using WSL, please clone the repo *inside* WSL, *not* on the Windows side (for
   performance reasons).

2. Run `git submodule update --init --recursive`

   Next, you'll need to acquire the **original 1.0 `main` NSO executable**.

    * To dump it from a Switch,
      follow [the instructions on the wiki](https://zeldamods.org/wiki/Help:Dumping_games#Dumping_binaries_.28executable_files.29).
    * You do not need to dump the entire game (RomFS + ExeFS + DLC). Just dumping the 1.0 ExeFS is sufficient.

3. Run `tools/setup.py [path to the NSO]`
    * This will:
        * install tools/check to check for differences in decompiled code
        * convert the executable if necessary
        * set up clang, ld.lld and other tools by downloading them from the releases of [OdysseyDecompToolsCache](https://github.com/MonsterDruide1/OdysseyDecompToolsCache/)
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

This decompilation uses [this](https://github.com/open-ead/sead) as a reference for the sead library used. Big thanks to
their research!
