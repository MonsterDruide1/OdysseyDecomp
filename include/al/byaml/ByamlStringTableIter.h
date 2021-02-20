#pragma once

namespace al
{
    class ByamlStringTableIter
    {
    public:
        ByamlStringTableIter();
        ByamlStringTableIter(const unsigned char *, bool);

        int getSize() const;
        const unsigned char* getAddressTable() const;
        int getStringAddress(int) const;
        int getEndAddress() const;
        const char* getString(int) const;
        int getStringSize(int) const;
        int findStringIndex(const char *) const;
        bool isValidate() const;

        const unsigned char* mData; // _0
        unsigned char _8;
    };
};