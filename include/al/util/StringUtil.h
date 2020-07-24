#pragma once

namespace al
{
    bool isEqualString(const char16_t *, const char16_t *);
    bool isEqualSubString(const char *, const char *);
    // isEqualSubString(const sead::SafeStringBase<char> &, const sead::SafeStringBase<char> &)
    bool isStartWithString(const char *, const char *);
    bool isEndWithString(const char *, const char *);
    bool isEqualString(const char *, const char *);
};