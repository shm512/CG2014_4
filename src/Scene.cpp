#include "Scene.h"

void
Scene::AddObject(
    char *fName,
    glm::vec3 center,
    float rotation_angle,
    float inclination_angle,
    glm::vec3 scale)
{
    static const glm::vec3 inclination_axis = glm::vec3(0, 1, 0);
    static const glm::vec3 rotation_axis = glm::vec3(1, 0, 0);
    glm::mat4x4 translateMatr = glm::translate(glm::mat4x4(), center);
    glm::mat4x4 inclinateMatr =
        glm::rotate(glm::mat4x4(), inclination_angle, inclination_axis);
    glm::mat4x4 rotateMatr =
        glm::rotate(inclinateMatr, rotation_angle, rotation_axis);
    glm::mat4x4 scaleMatr = glm::scale(glm::mat4x4(), scale);
    Object newObject(fName, translateMatr, rotateMatr, scaleMatr);
    objects.push_back(newObject);
}

const std::vector<Light> &
Scene::GetLightSources() const
{
    return lightSources;
}

const std::vector<Object> &
Scene::GetObjects() const
{
    return objects;
}

void
Scene::AddLightSource(const Light &lightSource)
{
    lightSources.push_back(lightSource);
}
