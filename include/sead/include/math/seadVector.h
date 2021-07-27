#ifndef SEAD_VECTOR_H_
#define SEAD_VECTOR_H_

#include <basis/seadTypes.h>
#include <cmath>
#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
struct Vector2 : public Policies<T>::Vec2Base
{
    /// @warning This constructor leaves member variables uninitialized.
    Vector2() {}
    Vector2(const Vector2& other) = default;
    constexpr Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    constexpr Vector2& operator=(const Vector2& other)
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }

    constexpr Vector2& operator+=(const Vector2& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    constexpr friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return {a.x + b.x, a.y + b.y};
    }

    constexpr Vector2& operator-=(const Vector2& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    constexpr friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    Vector2& operator*=(T t)
    {
        this->x *= t;
        this->y *= t;
        return *this;
    }

    constexpr friend Vector2 operator*(const Vector2& a, T t) { return {a.x * t, a.y * t}; }
    constexpr friend Vector2 operator*(T t, const Vector2& a) { return operator*(a, t); }

    constexpr Vector2& operator/=(T t)
    {
        this->x /= t;
        this->y /= t;
        return *this;
    }

    constexpr friend Vector2 operator/(const Vector2& a, T t) { return {a.x / t, a.y / t}; }
    constexpr friend Vector2 operator/(T t, const Vector2& a) { return operator/(a, t); }

    constexpr bool operator==(const Vector2& rhs) const
    {
        return this->x == rhs.x && this->y == rhs.y;
    }
    constexpr bool operator!=(const Vector2& rhs) const { return !operator==(rhs); }

    static const Vector2 zero;
    static const Vector2 ex;
    static const Vector2 ey;
    static const Vector2 ones;
};

template <typename T>
struct Vector3 : public Policies<T>::Vec3Base
{
    /// @warning This constructor leaves member variables uninitialized.
    Vector3() {}
    Vector3(const Vector3& other) = default;
    constexpr Vector3(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    constexpr Vector3& operator=(const Vector3& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    constexpr Vector3& operator+=(const Vector3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    constexpr friend Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    constexpr Vector3& operator-=(const Vector3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    constexpr friend Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    constexpr Vector3& operator*=(T t)
    {
        this->x *= t;
        this->y *= t;
        this->z *= t;
        return *this;
    }

    constexpr friend Vector3 operator*(const Vector3& a, T t)
    {
        return {a.x * t, a.y * t, a.z * t};
    }
    constexpr friend Vector3 operator*(T t, const Vector3& a) { return operator*(a, t); }

    constexpr Vector3& operator/=(T t)
    {
        this->x /= t;
        this->y /= t;
        this->z /= t;
        return *this;
    }

    constexpr friend Vector3 operator/(const Vector3& a, T t)
    {
        return {a.x / t, a.y / t, a.z / t};
    }
    constexpr friend Vector3 operator/(T t, const Vector3& a) { return operator/(a, t); }

    constexpr bool operator==(const Vector3& rhs) const
    {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    }
    constexpr bool operator!=(const Vector3& rhs) const { return !operator==(rhs); }

    void normalize();

    static const Vector3 zero;
    static const Vector3 ex;
    static const Vector3 ey;
    static const Vector3 ez;
    static const Vector3 ones;
};

template <typename T>
struct Vector4 : public Policies<T>::Vec4Base
{
    /// @warning This constructor leaves member variables uninitialized.
    Vector4() {}
    Vector4(const Vector4& other) = default;
    constexpr Vector4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    constexpr Vector4& operator=(const Vector4& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    constexpr Vector4& operator+=(const Vector4& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }

    constexpr friend Vector4 operator+(const Vector4& a, const Vector4& b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }

    constexpr Vector4& operator-=(const Vector4& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }

    constexpr friend Vector4 operator-(const Vector4& a, const Vector4& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }

    constexpr Vector4& operator*=(T t)
    {
        this->x *= t;
        this->y *= t;
        this->z *= t;
        this->w *= t;
        return *this;
    }

    constexpr friend Vector4 operator*(const Vector4& a, T t)
    {
        return {a.x * t, a.y * t, a.z * t, a.w * t};
    }
    constexpr friend Vector4 operator*(T t, const Vector4& a) { return operator*(a, t); }

    constexpr Vector4& operator/=(T t)
    {
        this->x /= t;
        this->y /= t;
        this->z /= t;
        this->w /= t;
        return *this;
    }

    constexpr friend Vector4 operator/(const Vector4& a, T t)
    {
        return {a.x / t, a.y / t, a.z / t, a.w / t};
    }
    constexpr friend Vector4 operator/(T t, const Vector4& a) { return operator/(a, t); }

    constexpr bool operator==(const Vector4& rhs) const
    {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
    }
    constexpr bool operator!=(const Vector4& rhs) const { return !operator==(rhs); }

    static const Vector4 zero;
    static const Vector4 ex;
    static const Vector4 ey;
    static const Vector4 ez;
    static const Vector4 ew;
    static const Vector4 ones;
};

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector4<f32> Vector4f;

template <>
const Vector2<f32> Vector2<f32>::zero;

template <>
const Vector2<f32> Vector2<f32>::ex;

template <>
const Vector2<f32> Vector2<f32>::ey;

template <>
const Vector2<f32> Vector2<f32>::ones;

template <>
const Vector3<f32> Vector3<f32>::zero;

template <>
const Vector3<f32> Vector3<f32>::ex;

template <>
const Vector3<f32> Vector3<f32>::ey;

template <>
const Vector3<f32> Vector3<f32>::ez;

template <>
const Vector3<f32> Vector3<f32>::ones;

template <>
const Vector4<f32> Vector4<f32>::zero;

template <>
const Vector4<f32> Vector4<f32>::ex;

template <>
const Vector4<f32> Vector4<f32>::ey;

template <>
const Vector4<f32> Vector4<f32>::ez;

template <>
const Vector4<f32> Vector4<f32>::ew;

template <>
const Vector4<f32> Vector4<f32>::ones;

template <typename T>
constexpr T dot(const Vector3<T>& u, const Vector3<T>& v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

template <typename T>
inline T norm2(const Vector3<T>& v)
{
    return std::sqrt(dot(v, v));
}

template <typename T>
inline void Vector3<T>::normalize()
{
    const auto norm = norm2(*this);
    if (norm > 0.0)
        *this *= 1.0 / norm;
}
}  // namespace sead

#endif  // #define SEAD_VECTOR_H_
