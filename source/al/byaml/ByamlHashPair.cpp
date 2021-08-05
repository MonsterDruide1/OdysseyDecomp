#include "al/byaml/ByamlHashPair.h"
#include "byteswap.h"

namespace al
{
    int ByamlHashPair::getKey(bool isRev) const
    {
        int key;

        if (isRev)
            key = (mData & 0xFF00) | (((mData & 0xFF) << 16) & 0xFFFFFF00) | ((mData >> 16) & 0xFF);
        else
            key = mData & 0xFFFFFF;
        
        return key;
    }

    char ByamlHashPair::getType() const
    {
        return mType;
    }

    int ByamlHashPair::getValue(bool isSwap) const
    {
        int swap = __bswap_32(mData);
        int val;

        if (isSwap)
            val = swap;
        else
            val = mData;
        
        return val;
    }
};