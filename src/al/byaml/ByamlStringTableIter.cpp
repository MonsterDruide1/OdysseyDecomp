#include "al/byaml/ByamlStringTableIter.h"
#include "byteswap.h"

namespace al
{
    ByamlStringTableIter::ByamlStringTableIter()
        : mData(0), mReversed(0) { }

    ByamlStringTableIter::ByamlStringTableIter(const u8* pData, bool pReversed)
    {
        mData = pData;
        mReversed = pReversed;
    }

    int ByamlStringTableIter::getSize() const
    {
        u32 type_and_size = *reinterpret_cast<const u32*>(mData);
        if(mReversed)
            return ((type_and_size >> 24) & 0xFF) | ((type_and_size >> 16) & 0xFF) << 8 | ((type_and_size >> 8) & 0xFF) << 16;
        return type_and_size >> 8;
    }

    const u32* ByamlStringTableIter::getAddressTable() const
    {
        // mData is an integer pointer, so getting to the table is just increasing the pointer by 1 (which is + 4)
        return reinterpret_cast<const u32*>(mData + 4);
    }

    int ByamlStringTableIter::getStringAddress(int idx) const
    {
        if (mReversed)
            return __bswap_32(getAddressTable()[idx]);
            
        return getAddressTable()[idx];
    }

    bool ByamlStringTableIter::isValidate() const
    {
        return mData != nullptr;
    }
};