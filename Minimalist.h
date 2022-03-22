#pragma once
#include "Vec.h"
#include "Ray.h"
#include <cmath>
#include <utility>
class Solid
{
public: // ctor & dtor 
    Solid() = default;
    Solid(Vec3d emt,  Vec4d col,  double glsy,  double mir): 
           emit{emt}, color{col}, glossy{glsy}, mirror{mir} { }
    virtual ~Solid() = default;
protected:
    Vec3d   emit;       //发光
    Vec4d   color;      //漫反射 + 透明度
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
public: //ctor & dtor
    Rect() = default;
    ~Rect() = default; 
    Rect(Vec3d emt, Vec4d col, double glsy, double mir, const Vec3d *vers):Solid(emt, col, glsy, mir)
    {   // 默认逆时针
        vertex[0] = vers[0];
        vertex[1] = vers[1];
        vertex[2] = vers[2];
        vertex[3] = vers[3];
        normal = (vertex[1] - vertex[0]).cross(vertex[2] - vertex[1]).unitlize();
    }
public: // facility 
    // 1.getNormal
    const Vec3d getNormal(const Vec3d &dir = Vec3d{}) override {return normal;}
    
    // 2.interscet
    virtual std::pair<double,Ray> intersect(const Ray &ray)
    { 
        if(std::fabs(ray.dir.dot(normal)) >= _tol) // 不与面平行，与法向量垂直
        {
            double t_hit = 0; // 算出使end + t*dir 位于面上的t
            // dir是单位向量，t_hit代表了end到hit point的距离
            // t = normal*(vertex[0] - end)/(normal*dir)
            t_hit = normal.dot(vertex[0] - ray.end) / (normal.dot(ray.dir)); 
            if(t_hit > 0)
            {
                auto p_hit = ray.end + t_hit * ray.dir;
                 // 检查是不是在里面
                double s  = (vertex[0] - vertex[2]).cross(vertex[0] - vertex[1]).norm();
                double s1 = (p_hit - vertex[0]).cross(p_hit - vertex[1]).norm();
                double s2 = (p_hit - vertex[1]).cross(p_hit - vertex[2]).norm();
                double s3 = (p_hit - vertex[2]).cross(p_hit - vertex[0]).norm();
                bool in_tri1 = fabs(s - s1 - s2 - s3) < _tol;

                s = (vertex[0] - vertex[2]).cross(vertex[0] - vertex[3]).norm();
                s1 = (p_hit - vertex[0]).cross(p_hit - vertex[2]).norm();
                s2 = (p_hit - vertex[2]).cross(p_hit - vertex[3]).norm();
                s3 = (p_hit - vertex[3]).cross(p_hit - vertex[0]).norm();
                bool in_tri2 = fabs(s - s1 - s2 - s3) < _tol;

                if (in_tri1 || in_tri2) // 唯一一种有交的情况
                {
                    Ray bounceRay{ray.end + t_hit * ray.dir, ray.dir - 2*normal.dot(ray.dir)*normal, 
                                  Vec3d{color.x*ray.col.x*normal.dot(-ray.dir), 
                                        color.y*ray.col.y*normal.dot(-ray.dir),
                                        color.z*ray.col.z*normal.dot(-ray.dir)},  // TODO 夹角相关
                                  ray.bounceTime+1};
                    if (!(emit==0))
                        bounceRay.hit_light = true;
                    return std::pair<double, Ray>{t_hit, bounceRay}; 
                }
            }
        }
        Ray bounceRay{ray.end, ray.dir, Vec3d{0,0,0}, 0};
        return std::pair<double, Ray>{__DBL_MAX__, bounceRay};   
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
    Sphere(Vec3d emt, Vec4d col, double glsy, double mir, const Vec3d &c, double r): 
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
        Ray bounceRay{ray.end + t_hit * ray.dir, ray.dir - 2*normal_hit.dot(ray.dir)*normal_hit, 
                      Vec3d{color.x*ray.col.x*normal_hit.dot(-ray.dir), 
                            color.y*ray.col.y*normal_hit.dot(-ray.dir),
                            color.z*ray.col.z*normal_hit.dot(-ray.dir)}, //TODO 夹角相关 
                      ray.bounceTime+1};
        if (!(emit==0))
            bounceRay.hit_light = true;
        return std::pair<double, Ray>{t_hit, bounceRay};    
    }
};