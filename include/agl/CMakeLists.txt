project(agl CXX ASM)

add_library(agl OBJECT
  include/agl/g3d/g3dResFile.h
  include/agl/Utils/aglAtomicPtrArray.h
  include/agl/Utils/aglParameter.h
  include/agl/Utils/aglParameterCurve.hpp
  include/agl/Utils/aglParameterIO.h
  include/agl/Utils/aglParameterList.h
  include/agl/Utils/aglParameterObj.h
  include/agl/Utils/aglParameterStringMgr.h
  include/agl/Utils/aglResCommon.h
  include/agl/Utils/aglResParameter.h
  src/Utils/aglAtomicPtrArray.cpp
  src/Utils/aglParameter.cpp
  src/Utils/aglParameterIO.cpp
  src/Utils/aglParameterList.cpp
  src/Utils/aglParameterObj.cpp
  src/Utils/aglParameterStringMgr.cpp
  src/Utils/aglResCommon.cpp
  src/Utils/aglResParameter.cpp
)

target_compile_options(agl PRIVATE -fno-exceptions)
target_compile_options(agl PRIVATE -fno-strict-aliasing)
target_compile_options(agl PRIVATE -Wno-invalid-offsetof)
target_include_directories(agl PUBLIC include/)

if(NOT TARGET sead)
  add_subdirectory(../sead)
endif()
target_link_libraries(agl PUBLIC sead)
