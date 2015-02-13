#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
#include "Types.h"
#include "Scene.h"

#include "string"

class Tracer
{
    Scene* pScene;
    Camera camera;
    Ray MakeRay(float screenPos_x, float screenPos_y); // Create ray from pixel of screen
    
    
    friend class TriangleTracer;
    
public:

    Tracer(Scene *_pScene, const Camera &_camera)
        : pScene(_pScene), camera(_camera)
    {}
    
    glm::vec3 TraceRay(Ray ray) const;           // Trace ray, compute its color
    
    const std::vector<Light> &GetLightSources() const;
    
    void RenderImage(int xRes, int yRes, float angleHoryz, float angleVert);
    
    void SaveImageToFile(std::string fileName) const;
};
