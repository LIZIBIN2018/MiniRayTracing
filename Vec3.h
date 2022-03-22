#pragma once

#include <iostream>
#include <cmath>

#pragma region 
#define VEC3_OPT_OVERRIDE(opt) \
Vec3 operator opt(const Vec3 &rhs) const \
{ \
    return Vec3<T>{x opt rhs.x, y opt rhs.y, z opt rhs.z};\
}

#define VEC3_OPT_MULDIV(opt) \
Vec3 operator opt(const T &rhs) const \
{ \
    return Vec3<T>{x opt rhs, y opt rhs, z opt rhs};\
}

#define VEC3_OPT_MULDIV_RIGHT(opt) \
template<class T>\
Vec3<T> operator opt(const T &scalar, const Vec3<T> &vec) \
{\
    return vec opt scalar; \
}

#define VEC3_OPT_OSTREAM \
template<class T> \
std::ostream &operator<<(std::ostream &os, Vec3<T> vec) \
{\
    os << '(' << vec.x << ',' << vec.y << ',' << vec.z << ')';\
    return os;\
}
#pragma endregion

template<class T>
struct Vec3
{
    T x,y,z;
public:
    Vec3(const Vec3 &rhs):x{rhs.x},y{rhs.y},z{rhs.z}{}
    Vec3(T xx = 0,T yy = 0,T zz = 0):x{xx},y{yy},z{zz}{}
    ~Vec3()=default;

public:
    // Vector operator
    VEC3_OPT_OVERRIDE(+)
    VEC3_OPT_OVERRIDE(-)

    // Vector and scalar operator
    VEC3_OPT_MULDIV(+)
    VEC3_OPT_MULDIV(-)
    VEC3_OPT_MULDIV(*)
    VEC3_OPT_MULDIV(/)    
public:
    // Vector geometry
    T       dot(const Vec3<T> &rhs) const { return x*rhs.x + y*rhs.y + z*rhs.z;}
    Vec3<T> cross(const Vec3<T> &rhs) const { return Vec3<T>{y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x}; }
    T       norm() const{ return std::sqrt(dot(*this));}
    Vec3<T> unitlize() const { return *this/norm();};

};
VEC3_OPT_OSTREAM

// Scalar and Vector operator 
VEC3_OPT_MULDIV_RIGHT(+)
VEC3_OPT_MULDIV_RIGHT(-)
VEC3_OPT_MULDIV_RIGHT(*)
VEC3_OPT_MULDIV_RIGHT(/)

#undef VEC3_OPT_OSTREAM
#undef VEC3_OPT_OVERRIDE
#undef VEC3_OPT_MULDIV
#undef VEC3_OPT_MULDEV_RIGHT

using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned char>;

