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

    if (!dot || dot < dirSeparator || ++dirSeparator == dot)
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

bool tryReplaceString(sead::BufferedSafeString* out, const char* oldStr, const char* newStr) {
    return tryReplaceString(out, out->cstr(), oldStr, newStr);
}

bool tryReplaceString(sead::BufferedSafeString* out, const char* targetStr, const char* oldStr,
                      const char* newStr) {
    const char* subStr = searchSubString(targetStr, oldStr);

    if (subStr == nullptr)
        return false;

    StringTmp<1024> before;
    StringTmp<1024> after;

    if (subStr != targetStr)
        before.copy(targetStr, subStr - targetStr);
    after.copy(subStr + std::strlen(oldStr));

    tryReplaceString(&after, oldStr, newStr);
    out->format("%s%s%s", before.cstr(), newStr, after.cstr());
    return true;
}

bool tryReplaceStringNoRecursive(sead::BufferedSafeString* out, const char* targetStr,
                                 const char* oldStr, const char* newStr) {
    const char* subStr = searchSubString(targetStr, oldStr);

    if (subStr == nullptr)
        return false;

    StringTmp<256> before;
    StringTmp<256> after;

    if (subStr != targetStr)
        before.copy(targetStr, subStr - targetStr);
    after.copy(subStr + std::strlen(oldStr));

    out->format("%s%s%s", before.cstr(), newStr, after.cstr());
    return true;
}

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

bool isMatchString(const char* str, const MatchStr& matchStr) {
    const char* subStr = getSubStringUnmatched(str, matchStr);

    if (subStr)
        return subStr[0] == '\0';

    return false;
}

s32 compareStringIgnoreCase(const char* str1, const char* str2) {
    return strcasecmp(str1, str2);
}

inline char toUpperCase(char ch) {
    // lowercase b: 0110 0010
    // uppercase B: 0100 0010
    return ch & 0b11011111;
}

inline bool isLetter(char ch) {
    return ch < 26u;
}

inline bool isPrintable(char ch) {
    bool isDigit = ch - '0' < 10u;

    return isLetter(toUpperCase(ch) - 'A') || isDigit;
}

// NON_MATCHING: Using inverse sub operation https://decomp.me/scratch/rPdbr
void makeUrlEncodeString(char* out, u32 len, const char* str) {
    out[0] = '\0';

    for (; *str != '\0'; str++) {
        char newSymbols[4] = {'\0', '\0', '\0', '\0'};
        s64 bytesWritten = 1;

        if (str[0] == ' ') {
            newSymbols[0] = '+';
        } else if (isPrintable(str[0])) {
            newSymbols[0] = str[0];
        } else {
            char digit = str[0] >> 4;
            char digit2 = str[0] & 0xf;

            newSymbols[0] = '%';
            newSymbols[1] = digit < 0xa ? digit + '0' : digit + 'A' - 10;
            newSymbols[2] = digit2 < 0xa ? digit2 + '0' : digit2 + 'A' - 10;
            bytesWritten = 3;
        }

        if (strlen(out) + bytesWritten > len) {
            out[len - 1] = '\0';
            return;
        }

        strcat(out, newSymbols);
    }
}

void makeUrlDecodeString(char* out, u32 len, const char* str) {
    out[0] = '\0';
    std::memset(out, 0, len);

    s32 bytesRead = 1;
    for (; str[0] != '\0'; str += bytesRead) {
        char newSymbols[2] = {'\0', '\0'};

        if (str[0] == '+') {
            newSymbols[0] = ' ';
            bytesRead = 1;
        } else if (str[0] == '%') {
            char offset1 = 10 - 'A';
            char offset2 = 10 - 'A';

            if (str[2] < 'A')
                offset2 = -'0';
            if (str[1] < 'A')
                offset1 = -'0';

            // hex string to char
            newSymbols[0] = (offset1 + str[1]) * 0x10 | (offset2 + str[2]);
            bytesRead = 3;
        } else {
            newSymbols[0] = str[0];
            bytesRead = 1;
        }

        if (strlen(out) + bytesRead > len) {
            out[len - 1] = '\0';
            return;
        }

        strcat(out, newSymbols);
    }
}

void copyString(char* out, const char* str, u32 len) {
    strncpy(out, str, len);
}

void copyStringW(char16* out, const char16* str, u32 len) {
    sead::StringUtil::wcs16cpy(out, len, str);
}

// Note: different cpp file from here?

// Attr required for createStringIfInStack
__attribute__((noinline)) bool isInStack(const void* element) {
    return sead::MemUtil::isStack(element);
}

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
