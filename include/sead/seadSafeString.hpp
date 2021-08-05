#include "seadSafeString.h"
#include <cstring>

namespace sead
{
    template <typename T>
    inline int SafeStringBase<T>::calcLength() const
    {
        assureTerminationImpl_();
        int length = 0;

        for (;;)
        {
            if (length > cMaximumLength || mTop[length] == cNullChar)
                break;

            length++;
        }

        if (length > cMaximumLength)
        {
            return 0;
        }

        return length;
    }

    template <typename T>
    inline int BufferedSafeStringBase<T>::copy(const SafeStringBase<T>& src, int copyLength)
    {
        T* dst = getMutableStringTop_();
        const T* csrc = src.cstr();
        if (dst == csrc)
            return 0;

        if (copyLength < 0)
            copyLength = src.calcLength();

        if (copyLength >= mSize)
        {
            copyLength = mSize - 1;
        }

        std::memcpy(dst, csrc, copyLength * sizeof(T));
        dst[copyLength] = SafeStringBase<T>::cNullChar;

        return copyLength;
    }
};