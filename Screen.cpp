#include "Screen.h"
#include <iostream>
#include <string>
#include <stb/stb_image_write.h>

Screen::Screen(int w, int h, Vec3d ep, double gs, double dst)
: width{w}, height{h}, eyePos{ep}, gridSize{gs}, dist{dst}
{   
    if(w < 0 || h < 0)
    {
        std::cerr << "Error: size cannot smaller than zero" << std::endl;
    }
    if(w == 0 || h == 0)
    {
        channals = nullptr;
        return;
    }
    channals = new unsigned char[w*h*3];
    return ;
}

Screen::~Screen()
{
    if(channals)
        delete[] channals;
    channals = nullptr;
}

void Screen::rendering(const Scene &extScene)
{
    scene = &extScene;
    double pix_x = 0;
    double pix_y = 0;
    for(int i = 0; i < width; ++i)
    {
        pix_x = i - int(width / 2);
        pix_x *= gridSize;
        for(int j = 0; j < height; ++j)
        {
            pix_y = j - int(height / 2);
            pix_y *= gridSize;
            Ray curRay{eyePos, Vec3d{pix_x, pix_y, -dist}.unitlize()};
            auto bounce = intersect(curRay);
            auto color = bounce.second.col;
            drawPixel(i,j,color.x,color.y,color.z);
        }
    }
}

std::pair<double, Ray> Screen::intersect(const Ray &ray)
{   
    double min_dist = __DBL_MAX__;
    std::pair<double, Ray> bounce;
    for(auto itr = scene->begin(); itr != scene->end();++itr)
    {
        auto bounce_temp = (*itr)->intersect(ray);
        if(bounce_temp.first < min_dist)
        {
            min_dist = bounce_temp.first;
            bounce = bounce_temp;
        }
    }
    return bounce;
}    

void Screen::output()
{
    std::string outputPath = "/home/phantom/coding/MiniRayTracing/raytracing.png";
    stbi_write_png(outputPath.c_str(), width, height, 3, channals, 0);   
}

void Screen::drawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    // stb是矩阵顺序，我是坐标顺序，换一下
    y = height - 1 - y;
    int place = width*y + x;
    place *= 3; // 3 channals

    channals[place  ] = r;
    channals[place+1] = g;
    channals[place+2] = b;
}