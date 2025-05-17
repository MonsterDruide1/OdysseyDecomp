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

void createFileNameBySuffix(sead::BufferedSafeString* out, const char* name, const char* suffix) {
    out->clear();
    if (!suffix) {
        out->append(name);
        return;
    }
    out->append(name);
    out->append(suffix);
}

u32 outputValueWithComma(char* out, u32 size, u64 value, bool usePadding, bool padToThousands) {
    if (value > 999999999) {
        return sead::StringUtil::snprintf(out, size, "%3d,%03d,%03d,%03d",
                                          (u32)(value / 1000000000), (u32)(value / 1000000 % 1000),
                                          (u32)(value / 1000 % 1000), (u32)(value % 1000));
    }
    if (value > 999999) {
        if (usePadding) {
            return sead::StringUtil::snprintf(out, size, "%3d,%03d,%03d", (u32)(value / 1000000),
                                              (u32)(value / 1000 % 1000), (u32)(value % 1000));
        }
        return sead::StringUtil::snprintf(out, size, "%d,%03d,%03d", (u32)(value / 1000000),
                                          (u32)(value / 1000 % 1000), (u32)(value % 1000));
    }
    if (value > 999) {
        if (usePadding) {
            if (padToThousands) {
                return sead::StringUtil::snprintf(out, size, "%3d,%03d", (u32)(value / 1000),
                                                  (u32)(value % 1000));
            }
            return sead::StringUtil::snprintf(out, size, "    %3d,%03d", (u32)(value / 1000),
                                              (u32)(value % 1000));
        }
        return sead::StringUtil::snprintf(out, size, "%d,%03d", (u32)(value / 1000),
                                          (u32)(value % 1000));
    }
    if (usePadding) {
        if (padToThousands)
            return sead::StringUtil::snprintf(out, size, "    %3d", (u32)value);
        return sead::StringUtil::snprintf(out, size, "        %3d", (u32)value);
    }
    return sead::StringUtil::snprintf(out, size, "%d", (u32)value);
}

void extractString(char* out, const char* str, u32 len, u32 unused) {
    strncpy(out, str, len);
    out[len] = '\0';
}

const char* searchSubString(const char* str, const char* substr) {
    return searchSubString(str, substr, strlen(substr));
}

const char* searchSubString(const char* str, const char* substr, s32 substrLen) {
    while (str[0] != '\0') {
        s32 size = 0;
        for (s32 index = 0; index < substrLen; index++) {
            if (str[index] == '\0' || str[index] != substr[index])
                break;
            size++;
        }

        if (size == substrLen)
            return str;

        str++;
    }

    return nullptr;
}

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

void translateCharacters(char* string, const char* charmap, const char* newCharmap) {
    while (charmap[0] != '\0') {
        for (s32 index = 0; string[index] != '\0'; index++)
            if (string[index] == *charmap)
                string[index] = *newCharmap;

        newCharmap++;
        charmap++;
    }
}

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

bool isEndWithString(const char* str, const char* end) {
    s32 lenStr = strlen(str);
    s32 lenEnd = strlen(end);

    if (lenStr < lenEnd)
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

// Attr required for isEndWithString
__attribute__((noinline)) bool isEqualString(const char* str1, const char* str2) {
    while (*str1 == *str2) {
        char val = *str1;

        if (!val)
            return true;

        str2++;
        str1++;
    }

    return false;
}

bool isEqualString(const sead::SafeString& safestr1, const sead::SafeString& safestr2) {
    const char* str1 = safestr1.cstr();
    const char* str2 = safestr2.cstr();
    while (*str1 == *str2) {
        char val = *str1;

        if (!val)
            return true;

        str2++;
        str1++;
    }

    return false;
}

bool isEqualStringCase(const char* str1, const char* str2) {
    return compareStringIgnoreCase(str1, str2) == 0;
}

bool isEqualStringCase(const sead::SafeString& str1, const sead::SafeString& str2) {
    return isEqualStringCase(str1.cstr(), str2.cstr());
}
}  // namespace al
