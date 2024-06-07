#include "Library/Base/StringUtil.h"

#include <resource/seadResource.h>

namespace al {

bool isEqualString(const char16_t* pString_0, const char16_t* pString_1) {
    u16 val;

    while (1) {
        val = *pString_0;
        if (val != *pString_1) {
            break;
        }

        ++pString_1;
        ++pString_0;

        if (!val) {
            return true;
        }
    }

    return false;
}

bool isEqualSubString(const char* pString_0, const char* pString_1) {
    return strstr(pString_0, pString_1);
}

bool isStartWithString(const char* pString_0, const char* pString_1) {
    if (!*pString_1)
        return true;

    while (*pString_0 && *pString_0 == *pString_1) {
        ++pString_0;
        ++pString_1;

        if (!*pString_1)
            return true;
    }

    return false;
}

bool isEqualString(const char* pString_0, const char* pString_1) {
    while (*pString_0 == *pString_1) {
        char val = *pString_0;  // required to match

        if (!val)
            return true;

        ++pString_1;
        ++pString_0;
    }

    return false;
}

bool isEndWithString(const char* pString_0, const char* pString_1) {
    s32 pString0_Len = strlen(pString_0);
    s32 pString1_Len = strlen(pString_1);

    if (pString0_Len < pString1_Len)
        return false;

    return isEqualString(&pString_0[pString0_Len - pString1_Len], pString_1);
}
}  // namespace al
