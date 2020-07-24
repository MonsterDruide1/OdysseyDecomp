#include "al/byaml/ByamlStringTableIter.h"
#include "aarch64.h"

namespace al
{
    ByamlStringTableIter::ByamlStringTableIter()
        : mData(0), _8(0) { }

    ByamlStringTableIter::ByamlStringTableIter(const unsigned char *pData, bool unk)
    {
        mData = (unsigned int*)pData;
        _8 = unk;
    }

    int ByamlStringTableIter::getSize() const
    {
        /* todo -- why does this use different register numbers? */
        return _8 != 0 ? ((*mData >> 24) & 0xFF) | ((*mData >> 16) & 0xFF) << 8 | ((*mData >> 8) & 0xFF) << 16 : *mData >> 8;
    }

    unsigned int* ByamlStringTableIter::getAddressTable() const
    {
        // mData is an integer pointer, so getting to the table is just increasing the pointer by 1 (which is + 4)
        return mData + 1;
    }

    int ByamlStringTableIter::getStringAddress(int idx) const
    {
        /* todo -- scheduling issues */
        int offs = *((int*)(mData + idx) + 1);
        int offsRev = bswap32(offs);        
        int ret;

        if (_8)
        {
            ret = offsRev;
        }
        else
        {
            ret = offs;
        }

        return ret;
    }

    bool ByamlStringTableIter::isValidate() const
    {
        return mData != nullptr;
    }
};