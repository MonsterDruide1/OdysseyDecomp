import pathlib, os, subprocess, sys

def error(str):
    print(str)
    sys.exit(1)

sdkRoot = os.getenv('SDK_ROOT')

if sdkRoot == None:
    error("Error: SDK_ROOT not set.")

root = pathlib.Path(sdkRoot)

compilerPath = root / "Compilers/NX/nx/aarch64/bin/clang++.exe"
includePath = root / "Include"
libraryPath = root / "Libraries/NX-NXFP2-a64/Release"
releasePath = root / "Common/Configs/Targets/NX-NXFP2-a64/Include"

compilerCommand = f"{compilerPath} -x c++ -std=gnu++14 -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -Wall -O3 -fomit-frame-pointer -mcpu=cortex-a57+fp+simd+crypto+crc -g -DNN_NINTENDO_SDK -DNN_SDK_BUILD_RELEASE -I include -I {includePath} -I {releasePath} -c "

source_folder = pathlib.Path('source/')
cpp_files = list(source_folder.rglob('*.cpp'))

numFiles = 0

for cpp_file in cpp_files:
    compilerCommand += str(cpp_file) + " "
    
if subprocess.call(compilerCommand) == 1:
    sys.exit(1)

print("Done.")