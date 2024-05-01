#include "Library/Base/Base.h"

namespace al {
s32 calcHashCode(char const* hash) {
    if (hash[0] == '\0') {
        return 0;
    }

    s32 hashCode = 0;
    for (s32 i = 0; hash[i] != '\0'; i++) {
        hashCode = (hashCode * 0x1f) + hash[i];
    }

    return hashCode;
}

s32 calcHashCodeLower(char const* hash) {
    s32 hashCode = 0;
    for (s32 i = 0; hash[i] != '\0'; i++) {
        hashCode = (hashCode * 0x1f) + tolower(hash[i]);
    }

    return hashCode;
}

s32 calcHashCodeFmt(char const* format, std::va_list argv) {
    char buf[0x100];
    vsnprintf(buf, 0x100, format, argv);

    return calcHashCode(buf);
}

s32 calcHashCodeFmt(char const* format, ...) {
    std::va_list argv;

    va_start(argv, format);
    s32 result = calcHashCodeFmt(format, argv);
    va_end(argv);

    return result;
}

const char* getBaseName(char const* name) {
    const char* baseName = strrchr(name, '/');

    return baseName != nullptr ? baseName + 1 : name;
}
}  // namespace al
