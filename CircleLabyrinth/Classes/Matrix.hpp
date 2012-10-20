#pragma once
#ifndef Matrix_hpp
#define Matrix_hpp

#include "Vector.hpp"

template <typename T>
struct Matrix2 {
    Matrix2()
    {
        x.x = 1; x.y = 0;
        y.x = 0; y.y = 1;
    }
    Matrix2(const T* m)
    {
        x.x = m[0]; x.y = m[1];
        y.x = m[2]; y.y = m[3];
    }
    Matrix2 operator * (const Matrix2& b) const
    {
        Matrix2 m;
        m.x.x = x.x * b.x.x + x.y * b.y.x;
        m.x.y = x.x * b.x.y + x.y * b.y.y;
        m.y.x = y.x * b.x.x + y.y * b.y.x;
        m.y.y = y.x * b.x.y + y.y * b.y.y;
        return m;
    }
    Vector2<T> operator * (const Vector2<T>& b) const
    {
        Vector2<T> v;
        v.x = x.x * b.x + x.y * b.y;
        v.y = y.x * b.x + y.y * b.y;
        return v;
    }
    Matrix2& operator *= (const Matrix2& b)
    {
        Matrix2 m = *this * b;
        return (*this = m);
    }
    static const Matrix2<T> CWRotationMatrix(const T angle)
    {
        const T c = cosf(angle);
        const T s = sinf(angle);
        return Matrix2<T>((T[]){c,s,-s,c});
    }
    static const Matrix2<T> CCWRotationMatrix(const T angle)
    {
        const T c = cosf(angle);
        const T s = sinf(angle);
        return Matrix2<T>((T[]){c,-s,s,c});
    }
    vec2 x;
    vec2 y;
};

typedef Matrix2<float> mat2;

#endif
