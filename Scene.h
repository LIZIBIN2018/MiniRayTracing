#pragma once 
#include <vector>
#include "Minimalist.h"

class Scene
{
    std::vector<Solid *> sceneVec;
public:
    Scene() = default;
    ~Scene()
    {
        for(auto &item: sceneVec)
            delete item;
        sceneVec.resize(0);
    }

public:
    void push_back(Solid *obj) { sceneVec.push_back(obj);}
    decltype(sceneVec.cbegin()) begin()  const {return sceneVec.cbegin();}
    decltype(sceneVec.cend())   end() const {return sceneVec.cend();}    
    decltype(sceneVec.begin()) begin() {return sceneVec.begin();}
    decltype(sceneVec.end())   end() {return sceneVec.end();}    
};