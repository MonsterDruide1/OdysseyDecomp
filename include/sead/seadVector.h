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

        static T zero;
        static T ex;
        static T ey;
        static T ones;
    };

    template<typename T>
    class Vector3
    {
    public:
        T x;
        T y;
        T z;

        inline void set(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        static T zero;
        static T ex;
        static T ey;
        static T ez;
        static T ones;
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

        static T zero;
        static T ex;
        static T ey;
        static T ez;
        static T ew;
        static T ones;
    };
};