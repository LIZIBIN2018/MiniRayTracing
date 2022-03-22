// Third party
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

// STD LIB
#include <iostream>
#include <string>
#include <vector>
// DEBUG TOOL
#include <time.h>
using namespace std;

// My own header
#include "Screen.h"
#include "Scene.h"

void create_scene(Scene &scene);
void free_scene(vector<Solid *> &scene);

int main()
{   
    Scene scene;
    create_scene(scene);

    int width = 1024;
    int height = width / 16 * 9;
    Screen screen(width, height, Vec3d{}, 0.6/double(width) ,0.6);
    screen.setScene(scene);

    clock_t start,end;  // TODELETE
    start = clock();  // TODELETE
    screen.rendering();
    end = clock();// TODELETE
    screen.output();
    
    double dur = end - start;// TODELETE
    dur /= CLOCKS_PER_SEC;// TODELETE
    cout << "渲染用了:" << dur << " 秒" << endl;// TODELETE
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
void add_sphere(Scene &scene, Vec3d emt, Vec4d rgba, double glsy, double mir, Vec3d cnt, double r)
{
    Sphere *sp = new Sphere{emt, rgba,glsy, mir, cnt, r };
    scene.push_back(sp);
}
void add_rectangle(Scene &scene, Vec3d emt, Vec4d rgba, double glsy, double mir, Vec3d *pos)
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
    Vec3d pos_light[4] = {   
        { 0.5, 2.248, depth_max+5.5},
        {-0.5, 2.248, depth_max+5.5},
        {-0.5, 2.248, depth_max+4.5},
        { 0.5, 2.248, depth_max+4.5}, };
    add_rectangle(scene, Vec3d{1 , 1, 1}, Vec4d{1,1,1,0}, 10, 0.5, pos_light);
    add_sphere(scene, Vec3d{0, 0, 0}, Vec4d{150.0/255,100.0/255,100.0/255,0}, 10, 1, Vec3d{-1.5 ,-1.45 , depth_max + 1},2);
    add_sphere(scene, Vec3d{0, 0, 0}, Vec4d{100.0/255,155.0/255,100.0/255,0}, 10, 1, Vec3d{ 0.5 ,-1.45 , depth_max + 2},2);
    Vec3d pos_t[4] = {   
        {  3, 2.25, -1},
        { -3, 2.25, -1},
        { -3, 2.25, depth_max},
        {  3, 2.25, depth_max}, };
    add_rectangle(scene, Vec3d{0 , 0, 0}, Vec4d{200.0/255,200.0/255,200.0/255,0}, 10, 0.5, pos_t);
    Vec3d pos_b[4] = 
    {
        { -3, -2.25, -1},
        {  3, -2.25, -1},
        {  3, -2.25, depth_max},
        { -3, -2.25, depth_max},
    };
    add_rectangle(scene, Vec3d{0 , 0, 0}, Vec4d{200.0/255,200.0/255,200.0/255,0}, 10, 0.5, pos_b);
    Vec3d pos_l[4] = 
    {
        { -3, -2.25, -1},
        { -3, -2.25, depth_max},
        { -3,  2.25, depth_max},
        { -3,  2.25, -1},
    };
    add_rectangle(scene, Vec3d{0 , 0, 0}, Vec4d{255.0/255,100.0/255,100.0/255,0}, 10, 0.5, pos_l);
    Vec3d pos_r[4] = 
    {
        { 3, -2.25, -1},
        { 3,  2.25, -1},
        { 3,  2.25, depth_max},
        { 3, -2.25, depth_max},
    };
    add_rectangle(scene, Vec3d{0 , 0, 0}, Vec4d{100.0/255,255.0/255,100.0/255,0},10,0.5,pos_r);
    Vec3d pos_be[4] = 
    {
        {  3, -2.25, depth_max},
        {  3,  2.25, depth_max},
        { -3,  2.25, depth_max},
        { -3, -2.25, depth_max},
    };
    add_rectangle(scene,Vec3d{0 , 0, 0}, Vec4d{150.0/255,150.0/255,150.0/255,0}, 10, 0.5, pos_be);
}