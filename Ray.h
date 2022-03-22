#pragma once 

#include "Vec.h"
#include <iostream>

struct Ray
{
    Vec3d end;
    Vec3d dir;
    Vec3u col;
    int bound;
    Ray() = default;
    Ray(Vec3d endPoint, Vec3d direction, Vec3u color = Vec3u{},int bd = 0): 
        end{endPoint}, dir{direction.unitlize()}, col{color}, bound{bd} { }
    ~Ray() = default;

    void show() 
    { 
        std::cout << "end point = " << end << std::endl;
        std::cout << "direction = " << dir << std::endl;
    }
};