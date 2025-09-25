#pragma once

#include <prim/seadSafeString.h>

namespace al {
struct MatchStr {
    const char* str;
};

const char* createStringIfInStack(const char* str);
const char* createConcatString(const char* start, const char* end);
void createFileNameBySuffix(sead::BufferedSafeString* out, const char* name, const char* suffix);
u32 outputValueWithComma(char* out, u32 size, u64 value, bool usePadding, bool padToThousands);
void extractString(char* out, const char* str, u32 len, u32 unused);
const char* searchSubString(const char* str, const char* substr);
const char* searchSubString(const char* str, const char* substr, s32 substrLen);
const char* getSubStringUnmatched(const char**, const char*, const MatchStr&,
                                  void (*)(const char*, const char*, void*), void*);
const char* getSubStringUnmatched(const char*, const MatchStr&);
void extractBaseNameW(sead::WBufferedSafeString*, const sead::WSafeString&);
void removeExtensionString(char* out, u32 len, const char* str);
void removeStringFromEnd(char* out, u32 len, const char* end, const char* str);
void translateCharacters(char* string, const char* charmap, const char* newCharmap);
void tryReplaceString(sead::BufferedSafeString*, const char*, const char*);
void tryReplaceString(sead::BufferedSafeString*, const char*, const char*, const char*);
void tryReplaceStringNoRecursive(sead::BufferedSafeString*, const char*, const char*, const char*);
bool isEqualString(const char16* str1, const char16* str2);
bool isEqualSubString(const char* str, const char* subStr);
bool isEqualSubString(const sead::SafeString& str, const sead::SafeString& subStr);
bool isStartWithString(const char* str, const char* start);
bool isEndWithString(const char* str, const char* end);
bool isMatchString(const char*, const MatchStr&);
s32 compareStringIgnoreCase(const char* str1, const char* str2);
void makeUrlEncodeString(char*, u32, const char*);
void makeUrlDecodeString(char*, u32, const char*);
void copyString(char* out, const char* str, u32 len);
void copyStringW(char16* out, const char16* str, u32 len);
bool isInStack(const void*);
bool isEqualString(const char* str1, const char* str2);
bool isEqualString(const sead::SafeString& safestr1, const sead::SafeString& safestr2);
bool isEqualStringCase(const char* str1, const char* str2);
bool isEqualStringCase(const sead::SafeString& str1, const sead::SafeString& str2);

template <s32 L>
class StringTmp : public sead::FixedSafeString<L> {
public:
    StringTmp(const char* format, ...) : sead::FixedSafeString<L>() {
        std::va_list args;
        va_start(args, format);
        sead::FixedSafeString<L>::formatV(format, args);
        va_end(args);
    }

    StringTmp() : sead::FixedSafeString<L>() {}
};

template <s32 L>
class WStringTmp : public sead::WFixedSafeString<L> {
public:
    WStringTmp(const char16* format, ...) : sead::WFixedSafeString<L>() {
        std::va_list args;
        va_start(args, format);
        sead::WFixedSafeString<L>::formatV(format, args);
        va_end(args);
    }

    WStringTmp() : sead::WFixedSafeString<L>() {}
};
}  // namespace al
