cmake_minimum_required(VERSION 3.13)
project(odyssey CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Use response files when linking objects because of the large number of source files
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS ON)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
  # Required to silence "unused argument" warnings for -stdlib=libc++ when using distcc
  add_compile_options(-Wno-unused-command-line-argument)
  # Required to fix source paths in debug info when using distcc
  add_compile_options(-fdebug-prefix-map=/tmp=${CMAKE_CURRENT_BINARY_DIR})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(odyssey)
target_include_directories(odyssey PRIVATE src)
target_compile_definitions(odyssey PRIVATE NON_MATCHING)
target_compile_options(odyssey PRIVATE -fno-rtti -fno-exceptions)
target_compile_options(odyssey PRIVATE -Wall -Wextra -Wdeprecated)
target_compile_options(odyssey PRIVATE -Wno-unused-parameter -Wno-unused-private-field)
target_compile_options(odyssey PRIVATE -fno-strict-aliasing)
target_compile_options(odyssey PRIVATE -Wno-invalid-offsetof)

set(NN_SDK_MAJOR 3)
set(NN_SDK_MINOR 5)
set(NN_SDK_PATCH 1)
set(NN_SDK_TYPE "Release")

set(NN_WARE_MAJOR 3)
set(NN_WARE_MINOR 5)
set(NN_WARE_PATCH 1)

add_subdirectory(src)
include_directories(lib)
include_directories(lib/aarch64)

add_subdirectory(lib/NintendoSDK)
target_link_libraries(odyssey PUBLIC NintendoSDK)

add_subdirectory(lib/sead)
target_link_libraries(odyssey PUBLIC sead)

add_subdirectory(lib/agl)
target_link_libraries(odyssey PUBLIC agl)
