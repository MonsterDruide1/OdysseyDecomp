#pragma once

namespace al {
class ByamlHeader {
public:
    short getTag() const;
    bool isInvertOrder() const;
    short getVersion() const;
    int getHashKeyTableOffset() const;
    int getStringTableOffset() const;
    int getDataOffset() const;

private:
    union {
        int _0;
        unsigned short mTag, mVersion;  // unusable due to different loading mechanisms
    };

    int mHashKeyOffset;
    int mStringTableOffset;
    int mDataOffset;
};
}  // namespace al
