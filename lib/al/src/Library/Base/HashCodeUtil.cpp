#include "Library/Base/HashCodeUtil.h"

#include <cctype>
#include <cstdio>
#include <cstring>

namespace al {
s32 calcHashCode(const char* str) {
    if (str[0] == '\0') {
        return 0;
    }

    s32 hashCode = 0;
    for (s32 i = 0; str[i] != '\0'; i++) {
        hashCode = (hashCode * 0x1f) + str[i];
    }

    return hashCode;
}

s32 calcHashCodeLower(const char* str) {
    s32 hashCode = 0;
    for (s32 i = 0; str[i] != '\0'; i++) {
        hashCode = (hashCode * 0x1f) + tolower(str[i]);
    }

    return hashCode;
}

s32 calcHashCodeFmt(const char* format, std::va_list argv) {
    char buf[0x100];
    vsnprintf(buf, 0x100, format, argv);

    return calcHashCode(buf);
}

s32 calcHashCodeFmt(const char* format, ...) {
    std::va_list argv;

    va_start(argv, format);
    s32 result = calcHashCodeFmt(format, argv);
    va_end(argv);

    return result;
}

const char* getBaseName(const char* name) {
    const char* baseName = strrchr(name, '/');

    return baseName != nullptr ? baseName + 1 : name;
}
}  // namespace al
