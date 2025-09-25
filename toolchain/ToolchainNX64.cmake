if (NOT HAS_WARNED_TOOLCHAIN AND (EXISTS "${CMAKE_CURRENT_LIST_DIR}/clang-3.9.1" OR EXISTS "${CMAKE_CURRENT_LIST_DIR}/clang-4.0.1"))
    message(WARNING "Full clang toolchains are no longer needed to build the project, please rerun setup.py!")
    # Only show this warning the first time cmake is reran by build.py
    set(HAS_WARNED_TOOLCHAIN 1 CACHE STRING "")
endif()

if (DEFINED ENV{ODYSSEY_CLANG})
    set(ODYSSEY_CLANG "$ENV{ODYSSEY_CLANG}")
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/bin/clang")
    set(ODYSSEY_CLANG "${CMAKE_CURRENT_LIST_DIR}/bin/clang")
else()
    set(ODYSSEY_CLANG "${CMAKE_CURRENT_LIST_DIR}/clang-3.9.1/bin/clang")
endif()

if (DEFINED ENV{ODYSSEY_CLANG_LLD})
    set(ODYSSEY_CLANG_LLD "$ENV{ODYSSEY_CLANG_LLD}")
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/bin/ld.lld")
    set(ODYSSEY_CLANG_LLD "${CMAKE_CURRENT_LIST_DIR}/bin/ld.lld")
else()
    set(ODYSSEY_CLANG_LLD "${CMAKE_CURRENT_LIST_DIR}/clang-4.0.1/bin/ld.lld")
endif()

set(NX64_OPT_FLAGS "-O3 -g")
set(NX64_TRIPLE aarch64-linux-elf)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_SYSROOT ${CMAKE_CURRENT_LIST_DIR}/musl)
set(CMAKE_C_COMPILER "${ODYSSEY_CLANG}")
set(CMAKE_C_COMPILER_TARGET ${NX64_TRIPLE})
set(CMAKE_CXX_COMPILER "${ODYSSEY_CLANG}")
set(CMAKE_CXX_COMPILER_TARGET ${NX64_TRIPLE})

set(CMAKE_C_FLAGS_RELEASE ${NX64_OPT_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${NX64_OPT_FLAGS})
set(CMAKE_C_FLAGS_RELWITHDEBINFO ${NX64_OPT_FLAGS})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${NX64_OPT_FLAGS})

# Target options
add_compile_options(-mcpu=cortex-a57+fp+simd+crypto+crc)
add_compile_options(-mno-implicit-float)
# Environment
add_compile_options(-stdlib=libc++)
add_compile_options(-fPIC)
# Helps with matching as this causes Clang to emit debug type info even for dynamic classes
# with undefined vtables.
add_compile_options(-fstandalone-debug)

add_definitions(-D SWITCH)
add_definitions(-D NNSDK)
add_definitions(-D MATCHING_HACK_NX_CLANG)

add_link_options(-stdlib=libc++ -nostdlib)
add_link_options(-fPIC -Wl,-Bsymbolic-functions -shared)
# Use lld for performance reasons (and because we don't want a dependency on GNU tools)
add_link_options(-fuse-ld=${ODYSSEY_CLANG_LLD})

include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/include)
