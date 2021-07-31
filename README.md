# OdysseyDecomp
Decompilation of all Super Mario Odyssey versions, from 1.0.0 to 1.3.0.

# Requirements
- Python 3.7

# Compiling
After downloading or cloning this repository, download [this.](http://shibboleet.us.to/compiler/compiler.7z) This contains the compiler that you will need to compile the actual game code. Place the zip on the root of the repository and extract it there. If done right, there should be a "compiler" folder on the root of the repository. After that is finished, run the "compile.bat", and it will compile every version of the game. The compiler output should be in "build/VER" where "VER" is the game version.

# Contributing
Anyone is welcome to contribute to this project, just send a pull request!

### TODO
- Different game versions with custom code sections should be included/excluded using `ifdefs`, so add the `VER_` definitions to the build chain when compiling specific verisions (example of usage: src/al/actor/Factory.cpp) - available definitions: `"VER_100", "VER_101", "VER_110", "VER_120", "VER_130"`.
- 1.3.0 uses a different optimization method, find it and implement it into the toolchain

# Credits
This decompilation uses [this](https://github.com/open-ead/sead) as a reference for the sead library used. Big thanks to their research!