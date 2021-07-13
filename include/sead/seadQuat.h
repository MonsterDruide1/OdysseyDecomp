#pragma once

namespace sead
{
    template<typename T>
    class Quat
    {
    public:
        T x;
        T y;
        T z;
        T w;

        static const Quat unit;
    };
};