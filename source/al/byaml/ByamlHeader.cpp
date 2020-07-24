#include "al/byaml/ByamlHeader.h"
#include "aarch64.h"

// "YB"
#define BYAML_LE_TAG 0x5942

namespace al
{
    short ByamlHeader::getTag() const
    {
        int revTag = bswap16(mTag);
        int ret;

        if (mTag == BYAML_LE_TAG)
            ret = revTag;
        else
            ret = mTag;
        
        return ret;
    }

    bool ByamlHeader::isInvertOrder() const
    {
        return mTag == BYAML_LE_TAG;
    }

    short ByamlHeader::getVersion() const
    {
        int tag = _0 & 0xFFFF;
        int ver = _0 >> 16;
        
        short revVer = bswap16(ver);
        short ret;

        // if the tag is "YB", then it is in little endian
        if (tag == BYAML_LE_TAG)
            ret = revVer;
        else
            ret = ver;
        
        return ret;
    }

    int ByamlHeader::getHashKeyTableOffset() const
    {
        short tag = mTag;
        int offsRev = bswap32(mHashKeyOffset);
        int offs;

        if (tag == BYAML_LE_TAG)
            offs = offsRev;
        else
            offs = mHashKeyOffset;
        
        return offs;
    }

    int ByamlHeader::getStringTableOffset() const
    {
        short tag = mTag;
        int offsRev = bswap32(mStringTableOffset);
        int offs;

        if (tag == BYAML_LE_TAG)
            offs = offsRev;
        else
            offs = mStringTableOffset;
        
        return offs;
    }

    int ByamlHeader::getDataOffset() const
    {
        short tag = mTag;
        int offsRev = bswap32(mDataOffset);
        int offs;

        if (tag == BYAML_LE_TAG)
            offs = offsRev;
        else
            offs = mDataOffset;
        
        return offs;
    }
};