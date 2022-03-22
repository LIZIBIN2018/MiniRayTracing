#pragma once 

#include <iostream>
#include <vector>
#include "Ray.h"
#include "Minimalist.h"
#include "Scene.h"

class Screen
{
    int width;    // stb是矩阵的序
    int height;
    Vec3d eyePos;
    double gridSize;
    double dist; // >=0 
    unsigned char *channals;
    const Scene *scene;
public:
    Screen(int w = 1,int h = 1, Vec3d ep = Vec3d{}, double gs = 0.001, double dst = 0.1);
    ~Screen();
    void rendering(const Scene &);
    void output();
    void drawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    std::pair<double, Ray> intersect(const Ray &r);
    

public: // debug func
    void show()
    {
        std::cout << "width = " << width << std::endl;
        std::cout << "height = " << height << std::endl;
        std::cout << "eyepos = " << eyePos << std::endl;
        std::cout << "gridsize = " << gridSize << std::endl;
        std::cout << "dist = " << dist << std::endl;
    }
};