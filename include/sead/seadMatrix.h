#pragma once

namespace sead
{
    template<typename T>
    class Matrix34
    {
    public:
        T mMtx[3][4];

        static const Matrix34 zero;
        static const Matrix34 ident;
    };
};