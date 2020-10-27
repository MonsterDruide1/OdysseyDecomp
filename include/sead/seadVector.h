#pragma once

namespace sead
{
    template<typename T>
    class Vector2
    {
    public:
        T x;
        T y;

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

        static T zero;
        static T ex;
        static T ey;
        static T ez;
        static T ew;
        static T ones;
    };
};