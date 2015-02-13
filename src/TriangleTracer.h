#pragma once

#include "Tracer.h"


class TriangleTracer
{
    Ray ray;
    const Tracer &tracer;
    
    glm::vec3
    convertCoordFromBary(const LTriangle2 &triangle, glm::vec3 baryPos) const;
    
    glm::vec3
    lighting(
        glm::vec3 position,
        glm::vec3 normal,
        const LMaterial &material,
        const std::vector<Light> &lightSources) const;
    
public:

    TriangleTracer(const Ray &_ray, const Tracer &_tracer);
    
    glm::vec3
    operator() (
        const LTriangle2 &triangle,
        const LMaterial &material,
        const Transform &transform) const;
};
