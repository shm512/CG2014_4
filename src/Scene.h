#pragma once

#include "Types.h"
#include "Object.h"

#include <vector>
#include <map>


class Scene
{
    std::vector<Light> lightSources;
    std::vector<Object> objects;
    
public:
    
    const std::vector<Light> &GetLightSources() const;
    
    const std::vector<Object> &GetObjects() const;
    
    void AddLightSource(const Light &lightSource);
    
    void AddObject(
        char *fName,
        glm::vec3 center,
        float rotation_angle,
        float inclination_angle,
        glm::vec3 scale
    );
};
