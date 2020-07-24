#pragma once

#include "al/byaml/ByamlHashPair.h"

namespace al
{
    class ByamlData
    {
    public:
        ByamlData();

        void set(const ByamlHashPair *, bool);
        void set(unsigned char, unsigned int, bool);
        unsigned char getType() const;
        unsigned int getValue() const;

        unsigned int mValue; // _0
        unsigned char mType; // _4
    };
};