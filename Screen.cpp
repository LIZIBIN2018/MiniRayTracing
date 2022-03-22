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

void Screen::rendering()
{
    double pix_x = 0;
    double pix_y = 0;
    for(int i = 0; i < width; ++i)
    {
        pix_x  = i - int(width / 2);
        pix_x *= gridSize;
        for(int j = 0; j < height; ++j)
        {
            pix_y = j - int(height / 2);
            pix_y *= gridSize;
            Ray castRay{eyePos, Vec3d{pix_x, pix_y, -dist}.unitlize()};
            
            auto bounce = intersect(castRay);
            auto color = bounce.second.col;
            drawPixel(i,j,color.x,color.y,color.z);
        }
    }
}

std::pair<double, Ray> Screen::intersect(const Ray &ray)
{   
    std::pair<double, Ray> bounce;
    bounce.second = ray;
    int max_time = 16; // 最多弹射次数(不要写成成员)//TODELETE //TOOD换成俄罗斯轮盘赌

    while(bounce.second.bounceTime < max_time && bounce.second.hit_light == false)
    {
        // 每次全场景求交都应该初始化
        double min_dist = __DBL_MAX__;   
        auto castRay = bounce.second;

        for (auto itr = scene->begin(); itr != scene->end(); ++itr) //  全场景求交 // TODO换成BVH 或者KD-tree
        {
            auto bounce_temp = (*itr)->intersect(castRay);
            if (bounce_temp.first < min_dist)
            {
                min_dist = bounce_temp.first;
                bounce = bounce_temp;
            }
        }
        // 跑出场景外
        if(min_dist == __DBL_MAX__)
        {
            bounce.second.col = Vec3d{};
            break; //不要循环啦
        }
    }
    if(bounce.second.hit_light == false)
        bounce.second.col=Vec3d{};
    return bounce;
}    

void Screen::output()
{
    std::string outputPath = "/home/phantom/coding/MiniRayTracing/raytracing.png";
    stbi_write_png(outputPath.c_str(), width, height, 3, channals, 0);   
}

void Screen::drawPixel(int x, int y, double r, double g, double b)
{
    // stb是矩阵顺序，我是坐标顺序，换一下
    y = height - 1 - y;
    int place = width*y + x;
    place *= 3; // 3 channals

    channals[place  ] = floor(r*255);
    channals[place+1] = floor(g*255);
    channals[place+2] = floor(b*255);
}