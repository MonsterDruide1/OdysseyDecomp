#pragma once

namespace al
{
    class ByamlContainerHeader
    {
    public:
        int getType() const;
        int getCount(bool) const;

        int mType; // _0
    };
};