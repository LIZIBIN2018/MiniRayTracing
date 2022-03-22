#pragma once
#include "Vec.h"
#include "Ray.h"
#include <cmath>
#include <utility>
class Solid
{
public: // ctor & dtor 
    Solid() = default;
    Solid(Vec3u emt,  Vec4u col,  double glsy,  double mir): 
           emit{emt}, color{col}, glossy{glsy}, mirror{mir} { }
    virtual ~Solid() = default;
protected:
    Vec3u   emit;       //发光
    Vec4u   color;      //漫反射 + 透明度
    double  glossy;     //镜面放射 
    double  mirror;     //镜面的比例
    virtual const Vec3d getNormal(const Vec3d &pos = Vec3d{}) = 0; 

    double _tol = 10e-6; // 精度容忍

public: // main function
    virtual std::pair<double,Ray> intersect(const Ray &ray) = 0;

public: // test function
    void showProp() const
    {
        std::cout << "emit = "   << emit << std::endl;
        std::cout << "color = "  << color << std::endl;
        std::cout << "glossy = " << glossy << std::endl;
        std::cout << "mirror = " << mirror << std::endl;
    }

};

class Rect: public Solid
{
    Vec3d vertex[4];  //顶点
    Vec3d normal;     //法线
public:
    Rect() = default;
    ~Rect() = default; 
    Rect(Vec3u emt, Vec4u col, double glsy, double mir, const Vec3d *vers):Solid(emt, col, glsy, mir)
    {   // 默认逆时针
        vertex[0] = vers[0];
        vertex[1] = vers[1];
        vertex[2] = vers[2];
        vertex[3] = vers[3];
        normal = (vertex[1] - vertex[0]).cross(vertex[2] - vertex[1]).unitlize();
    }
    const Vec3d getNormal(const Vec3d &dir = Vec3d{}) override {return normal;}
    virtual std::pair<double,Ray> intersect(const Ray &ray)
    { 
        if(std::fabs(ray.dir.dot(normal)) < _tol) // 与面平行，与法向量垂直
        {
            if(std::fabs((ray.end - vertex[0]).dot(normal)) < _tol) // 直接在面上
            {
                Ray bounceRay{ray.end, Vec3d{}, Vec3u{color.x, color.y,color.z}, 0};
                return std::pair<double, Ray>{0, bounceRay};
            }
            else
            {
                return std::pair<double, Ray>{__DBL_MAX__, ray};
            }
        }
        double t_hit = 0; // 算出end + t*dir 位于面上。
        // dir是单位向量，t_hit代表了end到hit point的距离
        // t = normal*(vertex[0] - end)/(normal*dir)
        t_hit = normal.dot(vertex[0] - ray.end) / (normal.dot(ray.dir));  
        if( t_hit < 0) // not hit
        {
            return std::pair<double, Ray>{__DBL_MAX__, ray};
        }
        
        Ray bounceRay{ray.end + t_hit * ray.dir, 2*normal.dot(ray.dir)*normal - ray.dir, Vec3u{color.x, color.y,color.z}, 0};
        return std::pair<double, Ray>{t_hit, bounceRay};    
    }
};

class Sphere: public Solid
{
    Vec3d   center; // 球心
    double  radius; // 半径
    Vec3d   normal(Vec3d dir);
public:
    Sphere()  = default;
    ~Sphere() = default;
    Sphere(Vec3u emt, Vec4u col, double glsy, double mir, const Vec3d &c, double r): 
            Solid(emt, col, glsy, mir), center(c), radius(r) { }
    
    const Vec3d getNormal(const Vec3d &pos = Vec3d{}) override
    {
        Vec3d tmp = pos - center;
        try
        {
            if(fabs(tmp.norm() - radius)>_tol)
            {
                throw pos;
            }
        }
        catch(Vec3d)
        {
            std::cerr << "Runtime Error: the point is not on the sphere, can not calculate the normal vector" << std::endl;
            return Vec3d{};
        }
        return tmp.unitlize();
    }   
    const Vec3d getNormal(double x, double y, double z)
    {
        return getNormal(Vec3d{x,y,z});
    }

    virtual std::pair<double,Ray> intersect(const Ray &ray)
    { 
        Vec3d distVec = center - ray.end - ((center - ray.end).dot(ray.dir))*ray.dir;
        if(distVec.norm()>radius-_tol) // not hit
        {
            return std::pair<double, Ray>{__DBL_MAX__, ray};
        }
        double a = std::pow(ray.dir.norm(),2);
        double b = 2*ray.dir.dot(ray.end - center);
        double c = std::pow((ray.end - center).norm(),2) - radius*radius;
        double t_hit = -b/(2*a) - std::sqrt(b*b - 4*a*c)/(2*a);

        if(t_hit < 0) // 可能在球里面，或者不hit
        {
            return std::pair<double, Ray>{__DBL_MAX__, ray};    
        }  

        Vec3d point_hit = ray.end + t_hit * ray.dir;
        Vec3d normal_hit = getNormal(point_hit);
        Ray bounceRay{ray.end + t_hit * ray.dir, 2*normal_hit.dot(ray.dir)*normal_hit - ray.dir, Vec3u{color.x, color.y,color.z}, 0};
        return std::pair<double, Ray>{t_hit, bounceRay};    
    }
};

