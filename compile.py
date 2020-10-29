import pathlib, os, shutil, subprocess, sys

def error(str):
    print(str)
    sys.exit(1)

versions = [ "VER_100", "VER_101", "VER_110", "VER_120", "VER_130" ]

gameVersion = versions[int(sys.argv[1])]

# todo -- 1.3.0 uses a different optimization method, find it
optimziation = "-O3"

root = pathlib.Path("compiler")
compilerPath = root / "nx/aarch64/bin/clang++.exe"
compilerCommand = f"{compilerPath} -x c++ -std=gnu++14 -fno-exceptions -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -D{gameVersion} -Wall {optimziation} -fomit-frame-pointer -mcpu=cortex-a57+fp+simd+crypto+crc -g -I include -c "

source_folder = pathlib.Path('source/')
cpp_files = list(source_folder.rglob('*.cpp'))

for cpp_file in cpp_files:
    compilerCommand += str(cpp_file) + " "
    
if subprocess.call(compilerCommand) == 1:
    sys.exit(1)

# this all assumes that the user doesn't have a single build folder
if not os.path.isdir("build/VER_100"):
    os.mkdir("build/VER_100")
    os.mkdir("build/VER_110")
    os.mkdir("build/VER_120")
    os.mkdir("build/VER_130")

# now let's copy our output to a folder called 'build'
cwd = pathlib.Path(os.getcwd())
buildDir = pathlib.Path(f'build/{gameVersion}/')
# our output is always in the root directory because it's the cwd
o_files = list(cwd.glob('*.o'))

for o_file in o_files:
    shutil.copy(o_file, buildDir)
    os.remove(o_file)

print("Done.")