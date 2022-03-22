#pragma once 

#include "Vec.h"
#include <iostream>

struct Ray
{
    Vec3d end;
    Vec3d dir;
    Vec3d col; // as white defaultly. if not hit light source, set zero.
    int bounceTime;
    bool hit_light = false;
    Ray() = default;
    Ray(Vec3d endPoint, Vec3d direction, Vec3d color = Vec3d{1.0,1.0,1.0},int bt = 0): 
        end{endPoint}, dir{direction.unitlize()}, col{color}, bounceTime{bt} { }
    ~Ray() = default;

    void show() 
    { 
        std::cout << "end point = " << end << std::endl;
        std::cout << "direction = " << dir << std::endl;
    }
};