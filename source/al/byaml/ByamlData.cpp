#include "al/byaml/ByamlData.h"
#include "aarch64.h"

namespace al
{
    ByamlData::ByamlData() :
        mValue(0), mType(0) { }

    void ByamlData::set(const ByamlHashPair *pPair, bool isRev)
    {
        mType = pPair->mType;
        int pairVal = pPair->mValue;
        int swapVal = bswap32(pairVal);

        if (isRev)
            pairVal = swapVal;

        mValue = pairVal;
    }

    void ByamlData::set(unsigned char type, unsigned int value, bool isRev)
    {
        int val = bswap32(value);

        if (!isRev)
            val = value;

        mType = type;
        mValue = val;
    }

    unsigned char ByamlData::getType() const
    {
        return mType;
    }

    unsigned int ByamlData::getValue() const
    {
        return mValue;
    }
};