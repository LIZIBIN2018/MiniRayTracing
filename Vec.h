#pragma once 

#include <iostream>
#include "Vec3.h"
#include "Vec4.h"

template<class T>
void convertVec3toVec4(const Vec3<T> &v3, Vec4<T> &v4) 
{
    v4.x = v3.x;
    v4.y = v3.y;
    v4.z = v3.z;
    v4.w = 1;
}

template<class T>
Vec4<T> convertVec3toVec4(const Vec3<T> &v3)
{
    return Vec4<T>{v3.x, v3.y, v3.z, 1};
}

template<class T>
void convertVec4toVec3(const Vec4<T> &v4, Vec3<T> &v3) 
{
    try{   
        if(v4.w == 0){
              throw "Runtime Error: 0 as denomiator.";
        }
    }
    catch (const char *errInfo)
    {
        std::cerr << errInfo << std::endl;
        return;
    }
    v3.x = v4.x / v4.w;
    v3.y = v4.y / v4.w;
    v3.z = v4.z / v4.w;
}


template<class T>
Vec3<T> convertVec4toVec3(const Vec4<T> &v4)
{
    try{
        if(v4.w == 0)
            throw "Runtime Error: 0 as denomiator.";
    }
    catch(const char *errInfo)
    {   
        std::cerr << errInfo << std::endl;
        return Vec3<T>();
    }
    return Vec3<T>{v4.x/v4.w, v4.y/v4.w , v4.z/v4.w}; 
}




