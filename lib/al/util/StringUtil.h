#pragma once

#include <prim/seadSafeString.h>

namespace al {

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

bool isEqualString(const char16_t*, const char16_t*);
bool isEqualSubString(const char*, const char*);
bool isEqualSubString(const sead::SafeStringBase<char>&, const sead::SafeStringBase<char>&);
bool isStartWithString(const char*, const char*);
bool isEndWithString(const char*, const char*);
bool isEqualString(const char*, const char*) __attribute__((noinline));
bool isEqualString(const sead::SafeString&, const sead::SafeString&);
};  // namespace al