#include "Library/Base/Base.h"

#include <cstring>

namespace al {
const char* getBaseName(const char* string) {
    const char* result = strrchr(string, '/');
    if (result)
        return result + 1;
    else
        return string;
}
}  // namespace al
