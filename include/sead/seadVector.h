#pragma once

namespace sead
{
    template<typename T>
    class Vector2
    {
    public:
        T x;
        T y;
    };

    template<typename T>
    class Vector3
    {
    public:
        T x;
        T y;
        T z;

        inline void set(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        static const Vector3 zero;
        static const Vector3 ex;
        static const Vector3 ey;
        static const Vector3 ez;
        static const Vector3 ones;
    };

    template<typename T>
    class Vector4
    {
    public:
        T x;
        T y;
        T z;
        T h;
        
        inline void set(T x, T y, T z, T h)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->h = h;
        }
    };
};