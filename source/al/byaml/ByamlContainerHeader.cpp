#include "al/byaml/ByamlContainerHeader.h"

namespace al
{
    int ByamlContainerHeader::getType() const
    {
        return mType;
    }

    int ByamlContainerHeader::getCount(bool isRev) const
    {
        /* todo -- attempt to remove ASR instruction for the else block */
        int val;

        if (isRev)
        {
            val = ((mType >> 24) & 0xFF) | ((mType >> 16) & 0xFF) << 8 | ((mType >> 8) & 0xFF) << 16;
        }
        else
        {
            val = mType >> 8;
        }
        
        return val;
    }
};