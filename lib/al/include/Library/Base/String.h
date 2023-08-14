#pragma once

#include <prim/seadSafeString.h>

namespace al {
void createStringIfInStack(const char*);
void createConcatString(const char*, const char*);
void createFileNameBySuffix(sead::BufferedSafeStringBase<char>*, const char*, const char*);
void outputValueWithComma(char*, u32, unsigned long, bool, bool);
void extractString(char*, const char*, u32, u32);
void searchSubString(const char*, const char*);
void searchSubString(const char*, const char*, s32);
void extractBaseNameW(sead::BufferedSafeStringBase<char16_t>*,
                      const sead::SafeStringBase<char16_t>&);
void removeExtensionString(char*, u32, const char*);
void removeStringFromEnd(char*, u32, const char*, const char*);
void translateCharacters(char*, const char*, const char*);
void tryReplaceString(sead::BufferedSafeStringBase<char>*, const char*, const char*);
void tryReplaceString(sead::BufferedSafeStringBase<char>*, const char*, const char*, const char*);
void tryReplaceStringNoRecursive(sead::BufferedSafeStringBase<char>*, const char*, const char*,
                                 const char*);
void compareStringIgnoreCase(const char*, const char*);
void makeUrlEncodeString(char*, u32, const char*);
void makeUrlDecodeString(char*, u32, const char*);
void copyString(char*, const char*, u32);
void copyStringW(char16_t*, const char16_t*, u32);
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
