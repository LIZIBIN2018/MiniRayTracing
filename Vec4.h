#pragma once

#include <iostream>

#pragma region 
#define VEC4_OPT_OVERRIDE(opt) \
Vec4 operator opt(const Vec4 &rhs) const \
{ \
    return Vec4<T>{x opt rhs.x, y opt rhs.y, z opt rhs.z, w opt rhs.w};\
}

#define VEC4_OPT_MULDIV(opt) \
Vec4 operator opt(const T &rhs) const \
{ \
    return Vec4<T>{x opt rhs, y opt rhs, z opt rhs, w opt rhs};\
}

#define VEC4_OPT_MULDIV_RIGHT(opt) \
template<class T>\
Vec4<T> operator opt(const T &scalar, const Vec4<T> &vec) \
{\
    return vec opt scalar; \
}

#define VEC4_OPT_OSTREAM \
template<class T> \
std::ostream &operator<<(std::ostream &os, Vec4<T> vec) \
{\
    os << '(' << vec.x << ',' << vec.y << ',' << vec.z << ',' << vec.w << ')';\
    return os;\
}
#pragma endregion

template<class T>
struct Vec4
{
   // friend class std::ostream;
    T x,y,z,w;
public:  //ctor & dtor
    Vec4(const Vec4 &rhs):x{rhs.x},y{rhs.y},z{rhs.z},w{rhs.w}{}
    Vec4(T xx = 0,T yy = 0,T zz = 0, T ww = 0):x{xx},y{yy},z{zz},w{ww}{}
    ~Vec4()=default;

public:
    VEC4_OPT_OVERRIDE(+)
    VEC4_OPT_OVERRIDE(-)
    VEC4_OPT_MULDIV(*)
    VEC4_OPT_MULDIV(/)    
};
VEC4_OPT_OSTREAM
VEC4_OPT_MULDIV_RIGHT(*)
VEC4_OPT_MULDIV_RIGHT(/)

#undef VEC4_OPT_OSTREAM
#undef VEC4_OPT_OVERRIDE
#undef VEC4_OPT_MULDIV
#undef VEC4_OPT_MULDEV_RIGHT

using Vec4d = Vec4<double>;
using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4u = Vec4<unsigned char>;

