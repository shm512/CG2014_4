#pragma once

#include "l3ds.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "InlineFunctions.hpp"

#include <vector>

struct Ray
{
    static const uint reflections_c = 0;
    
    glm::vec3 start;
    glm::vec3 dir;
    uint reflections_remain;
    
    Ray(
        glm::vec3 _start,
        glm::vec3 _dir,
        uint _reflections_remain = reflections_c
    ) :
        start(_start),
        dir(normalize(_dir)),
        reflections_remain(_reflections_remain)
    {}
    
    Ray(const Ray &rayOld, glm::mat4x4 transformVec, glm::mat4x4 transformDir)
        : Ray(
            transform_vec(transformVec, rayOld.start),
            transform_dir(transformDir, rayOld.dir),
            rayOld.reflections_c
        )
    {}
};

struct Camera
{
    glm::vec3 pos;          // Camera position and orientation
    glm::vec3 forward;      // Orthonormal basis
    glm::vec3 right;
    glm::vec3 up;

    glm::vec2 viewAngle;    // View angles, rad
    glm::uvec2 resolution;  // Image resolution: w, h

    std::vector<glm::vec3> pixels;  // Pixel array
};

struct Light
{
    glm::vec3 pos;
    
    float powerAmbient;
    float powerDiffuse;
    float powerSpecular;
    
    Light(
        glm::vec3 _pos,
        float _powerAmbient,
        float _powerDiffuse,
        float _powerSpecular
    ) :
        pos(_pos),
        powerAmbient(_powerAmbient),
        powerDiffuse(_powerDiffuse),
        powerSpecular(_powerSpecular)
    {}
};

struct Transform
{
    glm::mat4x4 model2global;
    glm::mat4x4 model2global_dir;
    glm::mat4x4 global2model;
    glm::mat4x4 global2model_dir;
    
    Transform(glm::mat4x4 translate, glm::mat4x4 rotate, glm::mat4x4 scale)
        :
        model2global(translate * rotate * scale),
        model2global_dir(rotate * scale),
        global2model(glm::inverse(model2global)),
        global2model_dir(glm::inverse(model2global_dir))
    {}
};
