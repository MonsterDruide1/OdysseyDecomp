#pragma once

namespace sead
{
    template<typename T>
    class Matrix34
    {
    public:
        union
        {
            T mtx[3][4];
            T _mtx[12];
        };

        static const Matrix34 ident;
    };
};