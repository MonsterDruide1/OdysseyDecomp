#pragma once

#include <prim/seadSafeString.h>

namespace al
{
    bool isEqualString(const char16_t *, const char16_t *);
    bool isEqualSubString(const char *, const char *);
    bool isEqualSubString(const sead::SafeStringBase<char> &, const sead::SafeStringBase<char> &);
    bool isStartWithString(const char *, const char *);
    bool isEndWithString(const char *, const char *);
    bool isEqualString(const char *, const char *);
};