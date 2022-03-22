#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>
#include <string>
#include <vector>
#include "Screen.h"
#include "Scene.h"

using namespace std;

void create_scene(Scene &scene);
void free_scene(vector<Solid *> &scene);

void generate_png()
{
    int w = 1200;
    int h = 900;
    int n = 3;
    unsigned char data[1200*900*3];
    for(int i = 0; i < 1200; i++)
    {
        for(int j = 0; j < 900; j++)
        {
            int place = i + w*j;
            place *= 3;
            data[place  ] = 255; 
            data[place+1] = static_cast<unsigned char>(floor(double(j)/900*255));
            data[place+2] = 0;
        }
    }
    stbi_write_png("/home/phantom/coding/raytracing/output.png",w,h,n,data,0);
}

int main()
{   
    //generate_png();
    Scene scene;
    create_scene(scene);

    int width = 1024;
    int height = width / 16 * 9;
    Screen screen(width, height, Vec3d{}, 0.6/double(width) ,0.6);
    screen.rendering(scene);
    screen.output();
    return 0;
}

void free_scene(vector<Solid *> &scene)
{
    for(auto item:scene)
    {
        delete item;
    }
    scene.resize(0);
}
void add_sphere(Scene &scene, Vec3u emt, Vec4u rgba, double glsy, double mir, Vec3d cnt, double r)
{
    Sphere *sp = new Sphere{emt, rgba,glsy, mir, cnt, r };
    scene.push_back(sp);
}

void add_rectangle(Scene &scene, Vec3u emt, Vec4u rgba, double glsy, double mir, Vec3d *pos)
{
    Rect *rct = new Rect{emt, rgba, glsy, mir, pos};
    scene.push_back(rct);
}

void create_scene(Scene &scene)
{
    // 眼睛放在原点
    // 视平面在0.6米远处
    // 宽高比 12：9   
    // 假设地板是y = -2.25，左右是x = +-3, 最近处是z = -1;
    
    double depth_max = -15;

    add_sphere(scene, Vec3u{0, 0, 0}, Vec4u{150,100,100,0}, 10, 1, Vec3d{-1.5 ,-1.45 , depth_max + 1},2);
    add_sphere(scene, Vec3u{0, 0, 0}, Vec4u{100,155,100,0}, 10, 1, Vec3d{ 0.5 ,-1.45 , depth_max + 2},2);
    Vec3d pos_t[4] = {   
        {  3, 2.25, -1},
        { -3, 2.25, -1},
        { -3, 2.25, depth_max},
        {  3, 2.25, depth_max}, };
    add_rectangle(scene, Vec3u{0 , 0, 0}, Vec4u{200,200,200,0}, 10, 0.5, pos_t);
    Vec3d pos_b[4] = 
    {
        { -3, -2.25, -1},
        {  3, -2.25, -1},
        {  3, -2.25, depth_max},
        { -3, -2.25, depth_max},
    };
    add_rectangle(scene, Vec3u{0 , 0, 0}, Vec4u{200,200,200,0}, 10, 0.5, pos_b);
    Vec3d pos_l[4] = 
    {
        { -3, -2.25, -1},
        { -3, -2.25, depth_max},
        { -3,  2.25, depth_max},
        { -3,  2.25, -1},
    };
    add_rectangle(scene, Vec3u{0 , 0, 0}, Vec4u{255,100,100,0}, 10, 0.5, pos_l);
    Vec3d pos_r[4] = 
    {
        { 3, -2.25, -1},
        { 3,  2.25, -1},
        { 3,  2.25, depth_max},
        { 3, -2.25, depth_max},
    };
    add_rectangle(scene, Vec3u{0 , 0, 0}, Vec4u{100,255,100,0},10,0.5,pos_r);
    Vec3d pos_be[4] = 
    {
        {  3, -2.25, depth_max},
        {  3,  2.25, depth_max},
        { -3,  2.25, depth_max},
        { -3, -2.25, depth_max},
    };
    add_rectangle(scene,Vec3u{0 , 0, 0}, Vec4u{150,150,150,0}, 10, 0.5, pos_be);
}

void vec3test()
{
    Vec3d v1{1,2,3};
    Vec3d v2{2,3,4};
    double scalar = 2;
    cout << v1 << endl; 
    cout << v2 << endl;
    cout << scalar*v1 << endl;
    cout << v2 + 2 << endl;
    cout << 2.0 + v2 << endl;
    cout << "corss and dot test: " << v1.dot(v2) << endl;
    cout << v1.cross(v2) << endl;
    cout << v1.dot(v1.cross(v2)) << endl;
    cout << v1.unitlize() << endl;
}

void vec4test()
{
    Vec4d v1{1,2,3,4};
    Vec4d v2{2,3,4,1};
    double scalar = 2;
    cout << v1 << endl; 
    cout << v2 << endl;
    cout << scalar*v1 << endl;
    cout << convertVec4toVec3(v2) << endl;
}