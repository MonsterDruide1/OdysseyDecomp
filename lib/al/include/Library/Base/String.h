#pragma once

#include <prim/seadSafeString.h>

namespace al {
void createStringIfInStack(const char*);
void createConcatString(const char*, const char*);
void createFileNameBySuffix(sead::BufferedSafeStringBase<char>*, const char*, const char*);
void outputValueWithComma(char*, unsigned int, unsigned long, bool, bool);
void extractString(char*, const char*, unsigned int, unsigned int);
void searchSubString(const char*, const char*);
void searchSubString(const char*, const char*, int);
void extractBaseNameW(sead::BufferedSafeStringBase<char16_t>*,
                      const sead::SafeStringBase<char16_t>&);
void removeExtensionString(char*, unsigned int, const char*);
void removeStringFromEnd(char*, unsigned int, const char*, const char*);
void translateCharacters(char*, const char*, const char*);
void tryReplaceString(sead::BufferedSafeStringBase<char>*, const char*, const char*);
void tryReplaceString(sead::BufferedSafeStringBase<char>*, const char*, const char*, const char*);
void tryReplaceStringNoRecursive(sead::BufferedSafeStringBase<char>*, const char*, const char*,
                                 const char*);
void compareStringIgnoreCase(const char*, const char*);
void makeUrlEncodeString(char*, unsigned int, const char*);
void makeUrlDecodeString(char*, unsigned int, const char*);
void copyString(char*, const char*, unsigned int);
void copyStringW(char16_t*, const char16_t*, unsigned int);
bool isInStack(const void*);
bool isEqualStringCase(const char*, const char*);
bool isEqualString(const char16_t*, const char16_t*);
bool isEqualSubString(const char*, const char*);
bool isEqualSubString(const sead::SafeString&, const sead::SafeString&);
bool isStartWithString(const char*, const char*);
bool isEndWithString(const char*, const char*);
bool isEqualString(const char*, const char*) __attribute__((noinline));
bool isEqualString(const sead::SafeString&, const sead::SafeString&);
bool isEqualStringCase(const sead::SafeString&, const sead::SafeString&);

template <s32 L>
class StringTmp : public sead::FixedSafeString<L> {  // equal to WFormatFixedSafeString
public:
    StringTmp(const char* format, ...) : sead::FixedSafeString<L>() {
        std::va_list args;
        va_start(args, format);
        this->formatV(format, args);
        va_end(args);
    }
    ~StringTmp() = default;
};
}  // namespace al
