#include "Library/Base/StringUtil.h"

#include <prim/seadStringUtil.h>

namespace al {
const char* createStringIfInStack(const char* str) {
    if (!isInStack(str))
        return str;

    s32 len = strlen(str) + 1;
    char* buffer = new char[len];
    snprintf(buffer, len, "%s", str);

    return buffer;
}

const char* createConcatString(const char* start, const char* end) {
    s32 len = strlen(start) + strlen(end) + 1;
    char* buffer = new char[len];
    snprintf(buffer, len, "%s%s", start, end);

    return buffer;
}

// void createFileNameBySuffix(sead::BufferedSafeString*, const char*, const char*)
// void outputValueWithComma(char*, u32, u64, bool, bool)

void extractString(char* out, const char* str, u32 len, u32 unused) {
    strncpy(out, str, len);
    out[len] = '\0';
}

// void searchSubString(const char*, const char*);
// void searchSubString(const char*, const char*, s32);
// const char* getSubStringUnmatched(const char**, const char*, const MatchStr&,
//                                   void (*)(const char*, const char*, void*), void*);
// const char* getSubStringUnmatched(const char*, const MatchStr&);
// void extractBaseNameW(sead::WBufferedSafeString*, const sead::WSafeString&);

void removeExtensionString(char* out, u32 len, const char* str) {
    snprintf(out, len, "%s", str);
    char* dot = strchr(out, '.');
    char* dirSeparator = strchr(out, '/');

    if (dot == nullptr || dot < dirSeparator || ++dirSeparator == dot)
        return;

    *dot = '\0';
}

void removeStringFromEnd(char* out, u32 len, const char* end, const char* str) {
    snprintf(out, len, "%s", str);

    s32 lenStr = strlen(out);
    s32 lenEnd = strlen(end);

    if (lenEnd > lenStr)
        return;

    out[lenStr - lenEnd] = '\0';
}

// void translateCharacters(char*, const char*, const char*);
// void tryReplaceString(sead::BufferedSafeString*, const char*, const char*);
// void tryReplaceString(sead::BufferedSafeString*, const char*, const char*, const char*);
// void tryReplaceStringNoRecursive(sead::BufferedSafeString*, const char*, const char*,
//                                  const char*);

bool isEqualString(const char16* str1, const char16* str2) {
    while (*str1 == *str2) {
        char16 val = *str1;

        if (!val)
            return true;

        str2++;
        str1++;
    }

    return false;
}

bool isEqualSubString(const char* str, const char* subStr) {
    return strstr(str, subStr) != nullptr;
}

bool isEqualSubString(const sead::SafeString& str, const sead::SafeString& subStr) {
    return isEqualSubString(str.cstr(), subStr.cstr());
}

bool isStartWithString(const char* str, const char* start) {
    if (*start == '\0')
        return true;

    while (*str != '\0' && *str == *start) {
        str++;
        start++;

        if (*start == '\0')
            return true;
    }

    return false;
}

// NON_MATCHING: inlined return https://decomp.me/scratch/XEtRH
bool isEndWithString(const char* str, const char* end) {
    s32 lenStr = strlen(str);
    s32 lenEnd = strlen(end);

    if (lenEnd > lenStr)
        return false;

    return isEqualString(&str[lenStr - lenEnd], end);
}

// bool isMatchString(const char*, const MatchStr&);

s32 compareStringIgnoreCase(const char* str1, const char* str2) {
    return strcasecmp(str1, str2);
}

// void makeUrlEncodeString(char*, u32, const char*);
// void makeUrlDecodeString(char*, u32, const char*);

void copyString(char* out, const char* str, u32 len) {
    strncpy(out, str, len);
}

void copyStringW(char16* out, const char16* str, u32 len) {
    sead::StringUtil::wcs16cpy(out, len, str);
}

// bool isInStack(const void*);

bool isEqualString(const char* str1, const char* str2) {
    while (*str1 == *str2) {
        char val = *str1;

        if (!val)
            return true;

        str2++;
        str1++;
    }

    return false;
}

bool isEqualString(const sead::SafeString& str1, const sead::SafeString& str2) {
    return isEqualString(str1.cstr(), str2.cstr());
}

bool isEqualStringCase(const char* str1, const char* str2) {
    return compareStringIgnoreCase(str1, str2) == 0;
}

bool isEqualStringCase(const sead::SafeString& str1, const sead::SafeString& str2) {
    return isEqualStringCase(str1.cstr(), str2.cstr());
}
}  // namespace al
