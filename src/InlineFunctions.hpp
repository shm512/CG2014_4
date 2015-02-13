#pragma once

static inline glm::vec3
normalize(glm::vec3 vec)
{
    return glm::length(vec) ? glm::normalize(vec) : vec;
}

static inline glm::vec3
lvec2glmvec(const LVector3 &lvec)
{
    return glm::vec3(lvec.x, lvec.y, lvec.z);
}    

static inline glm::vec3
lvec2glmvec(const LVector4 &lvec)
{
    return glm::vec3(lvec.x, lvec.y, lvec.z);
}

static inline glm::vec3
color2glmvec(const LColor3 &color)
{
    return glm::vec3(color.r, color.g, color.b);
}

static inline glm::vec3
transform_vec(glm::mat4x4 transformMatrix, glm::vec3 vec)
{
    return glm::vec3(transformMatrix * glm::vec4(vec, 1.0f));
}

static inline glm::vec3
transform_dir(glm::mat4x4 transformMatrix, glm::vec3 dir)
{
    return glm::vec3(transformMatrix * glm::vec4(dir, 0.0f));
}  
