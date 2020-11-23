#pragma once

namespace sead
{
    template<typename T>
    class Vector2
    {
    public:
        T x;
        T y;

        inline void set(T _x, T _y)
        {
            x = _x;
            y = _y;
        }

        static Vector2<T> zero;
        static Vector2<T> ex;
        static Vector2<T> ey;
        static Vector2<T> ones;
    };

    template<typename T>
    class Vector3
    {
    public:
        T x;
        T y;
        T z;

        inline Vector3(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        inline void set(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        const static Vector3 zero;
        const static Vector3 ex;
        const static Vector3 ey;
        const static Vector3 ez;
        const static Vector3 ones;
    };

    template<typename T>
    class Vector4
    {
    public:
        T x;
        T y;
        T z;
        T w;

        inline void set(T _x, T _y, T _z, T _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        static Vector4<T> zero;
        static Vector4<T> ex;
        static Vector4<T> ey;
        static Vector4<T> ez;
        static Vector4<T> ew;
        static Vector4<T> ones;
    };
};