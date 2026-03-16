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
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES_GAME CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES_AL CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/lib/al/*.cpp ${PROJECT_SOURCE_DIR}/lib/al/*.c ${PROJECT_SOURCE_DIR}/lib/al/*.h ${PROJECT_SOURCE_DIR}/lib/al/*.hpp)

add_executable(odyssey ${SOURCES_GAME} ${SOURCES_AL})

target_include_directories(odyssey PRIVATE src)
target_include_directories(odyssey PRIVATE lib/al)
target_compile_definitions(odyssey PRIVATE NON_MATCHING)
target_compile_options(odyssey PRIVATE -fno-exceptions)
target_compile_options(odyssey PRIVATE -Wall -Wextra -Wdeprecated)
target_compile_options(odyssey PRIVATE -Wno-unused-parameter -Wno-unused-private-field)
target_compile_options(odyssey PRIVATE -fno-strict-aliasing)
target_compile_options(odyssey PRIVATE -Wno-invalid-offsetof)
target_compile_options(odyssey PRIVATE -Wshadow)
target_compile_options(odyssey PRIVATE -Woverloaded-virtual)

set(NN_WARE 3.5.1)
set(NN_SDK 3.5.1)
set(NN_SDK_TYPE "Release")
add_compile_definitions(SEAD_VERSION=SEAD_VERSION_SMO)

include_directories(lib)
include_directories(lib/aarch64)

add_subdirectory(lib/NintendoSDK-NEX)
add_subdirectory(lib/NintendoSDK)
add_subdirectory(lib/sead)
add_subdirectory(lib/agl)
add_subdirectory(lib/eui)
target_link_libraries(odyssey PUBLIC NintendoSDK-NEX NintendoSDK sead agl eui)
