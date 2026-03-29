#pragma once

#include <basis/seadTypes.h>
#include <cstdarg>

namespace al {
u32 calcHashCode(const char* str);
u32 calcHashCodeLower(const char* str);
u32 calcHashCodeFmt(const char* format, std::va_list argv);
u32 calcHashCodeFmt(const char* format, ...);
const char* getBaseName(const char* name);
}  // namespace al
