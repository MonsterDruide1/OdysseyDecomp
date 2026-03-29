#pragma once

#include <basis/seadTypes.h>
#include <cstdarg>

namespace al {
u32 calcHashCode(const char* str);
s32 calcHashCodeLower(const char* str);
s32 calcHashCodeFmt(const char* format, std::va_list argv);
s32 calcHashCodeFmt(const char* format, ...);
const char* getBaseName(const char* name);
}  // namespace al
