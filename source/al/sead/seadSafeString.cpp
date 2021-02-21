#include "sead/seadSafeString.h"
#include <cstring>

namespace sead
{
    template <>
    const char SafeStringBase<char>::cNullChar = '\0';

    template <>
    void BufferedSafeStringBase<char>::assureTerminationImpl_() const
    {
        auto* mutableSafeString = const_cast<BufferedSafeStringBase<char>*>(this);
        mutableSafeString->getMutableStringTop_()[mSize - 1] = cNullChar;
    }
};