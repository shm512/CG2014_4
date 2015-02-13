#include "TriangleTracer.h"

using namespace std;


glm::vec3
TriangleTracer::convertCoordFromBary(const LTriangle2 &triangle,
                                     glm::vec3 baryPos) const
{
    glm::vec3 a = lvec2glmvec(triangle.vertices[0]);
    glm::vec3 b = lvec2glmvec(triangle.vertices[1]);
    glm::vec3 c = lvec2glmvec(triangle.vertices[2]);
    return (1 - baryPos.x - baryPos.y) * a + baryPos.x * b + baryPos.y * c;
}

glm::vec3
TriangleTracer::lighting(
    glm::vec3 position,
    glm::vec3 normal,
    const LMaterial &material,
    const vector<Light> &lightSources) const
{
    //Phong Reflection Model. Understandable description can be found here:
    //http://compgraphics.info/3D/lighting/phong_reflection_model.php
    
    //Everything here is in world coordinates!
    
    glm::vec3 V = normalize(-position);
    glm::vec3 N = normalize(normal);
    float LightPower_ambient = 0.0f;
    float LightPower_diffuse = 0.0f;
    float LightPower_specular = 0.0f;
    
    for (auto &light : lightSources) {
        glm::vec3 L = normalize(light.pos - position);
        LightPower_ambient += light.powerAmbient;
        LightPower_diffuse += max(glm::dot(N, L), 0.0f) * light.powerDiffuse;
        float dot_R_V = 2.0f * glm::dot(N, L) * glm::dot(N, V) - glm::dot(L, V);
        LightPower_specular +=
            pow(max(dot_R_V, 0.0f), material.GetShininess()) * light.powerSpecular;
    }
    
    glm::vec3 I_ambient =
        color2glmvec(material.GetAmbientColor()) * LightPower_ambient;
    glm::vec3 I_diffuse =
        color2glmvec(material.GetDiffuseColor()) * LightPower_diffuse;
    glm::vec3 I_specular =
        color2glmvec(material.GetSpecularColor()) * LightPower_specular;
        
    return I_ambient + I_diffuse + I_specular;        
}      

glm::vec3
interpolateNormal(glm::vec3 aNormal, glm::vec3 bNormal, glm::vec3 cNormal, glm::vec3 baryPos)
{
    glm::vec3 normal =
        (1 - baryPos.x - baryPos.y) * aNormal + baryPos.x * bNormal + baryPos.y * cNormal;
    return normal;
    //return normalize(normal);
}
    
//public:

TriangleTracer::TriangleTracer(const Ray &_ray, const Tracer &_tracer)
    : ray(_ray), tracer(_tracer)
{}

glm::vec3
TriangleTracer::operator() (
    const LTriangle2 &triangle,
    const LMaterial &material,
    const Transform &transform) const
{
    glm::vec3 worldPos, normal, baryPos;  //of intersection point
    if (glm::intersectRayTriangle(
            ray.start, ray.dir,
            lvec2glmvec(triangle.vertices[0]),
            lvec2glmvec(triangle.vertices[1]),
            lvec2glmvec(triangle.vertices[2]),
            baryPos)
        )
    {
        worldPos = 
            transform_vec(
                transform.model2global, 
                convertCoordFromBary(triangle, baryPos)
            );
        normal =
            transform_vec(
                transform.model2global_dir,
                interpolateNormal(
                    lvec2glmvec(triangle.vertexNormals[0]),
                    lvec2glmvec(triangle.vertexNormals[1]),
                    lvec2glmvec(triangle.vertexNormals[2]),
                    baryPos
                )
            );
        glm::vec3 texture = glm::vec3(0, 0, 1); //TODO: implement texture loading
        glm::vec3 color =
            lighting(worldPos, normal, material, tracer.GetLightSources()) * texture;
        if (ray.reflections_c > 0) {
            glm::vec3 reflection_dir = 
                glm::reflect(
                    transform_dir(transform.model2global_dir, ray.dir),
                    normal
                );
            tracer.TraceRay(
                Ray(worldPos, reflection_dir, ray.reflections_remain - 1)
            );
        }
        return color;
    } else {
        //no intersection
        return glm::vec3(0, 0, 0);
    }
}
