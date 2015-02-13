#pragma once

#include "Types.h"

#include <map>
#include <memory>
#include <tuple>


class Object
{
    static std::map<const char *, std::shared_ptr<L3DS>> l3dsObjectsDict;
    std::shared_ptr<L3DS> l3dsObject;
    
    Transform transform;
    
    std::tuple<glm::vec3, float> boundingSphere;
    
    glm::vec3 centerOfMass() const;
    
    float longestDistance(glm::vec3 center) const;
    
public:

    Object(
        const char *fName,
        glm::mat4x4 translate,
        glm::mat4x4 rotate,
        glm::mat4x4 scale
    );
        
    const Transform & GetTransform() const;
    
    std::tuple<glm::vec3, float> GetBoundingSphere() const;
        
    template<class ObjectMeshOperator>
    typename std::result_of<ObjectMeshOperator(
        const LTriangle2 &,
        const LMaterial &,
        const Transform &
    )>::type
    mesh_op(const ObjectMeshOperator &op) const;
};

//template function implementation
#include "Object.hpp"
