#include "Tracer.h"
#include "TriangleTracer.h"
#include "lodepng.h"

#include <tuple>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;


const vector<Light> &
Tracer::GetLightSources() const
{
    return pScene->GetLightSources();
}

Ray
Tracer::MakeRay(float screenPos_x, float screenPos_y)
{    
    float horyz = camera.resolution.x / 2.0;
    float vert = camera.resolution.y / 2.0;
    float dir_x = (screenPos_x - horyz) / horyz;    //tan(dir) in Oxz
    float dir_y = (screenPos_y - vert) / vert;      //tan(dir) in Oyz
    return Ray(
        camera.pos, 
        dir_x * camera.right + dir_y * camera.up + camera.forward //geometry!
        //{camera.right; camera.up; camera.forward} is basis
    );
}

glm::vec3
Tracer::TraceRay(Ray ray) const
{
    static const glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
    static const glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 center, position, normal;
    float boundRadius;
    for (auto &object : pScene->GetObjects()) {
        tie(center, boundRadius) = object.GetBoundingSphere();
        auto transform = object.GetTransform();
        //center = transform_vec(transform.model2global, center);
        Ray rayModel =
            Ray(ray, transform.global2model, transform.global2model_dir);
        if (glm::intersectRaySphere(
                rayModel.start, rayModel.dir,
                center, boundRadius,
                position, normal)   //position and normal won't be used
            )
        {
            //return white;
            return object.mesh_op(TriangleTracer(rayModel, *this));
        }
    }
    
    return black;
}

void
Tracer::RenderImage(int xRes, int yRes, float angleHoryz, float angleVert)
{
    camera.viewAngle = glm::vec2(angleHoryz, angleVert);
    camera.resolution = glm::uvec2(xRes, yRes);
    camera.pixels.resize(xRes * yRes);
    
    camera.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.forward = glm::vec3(0.0f, 0.0f, 1.0f);
    
    camera.right = glm::vec3(1.0f, 0.0f, 0.0f);
    assert(glm::length(camera.right) == glm::length(camera.forward));
    camera.right *= tan(angleHoryz / 2.0);
    
    camera.up = glm::vec3(0.0f, -1.0f, 0.0f);
    assert(glm::length(camera.up) == glm::length(camera.forward));
    camera.up *= tan(angleVert / 2.0);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < yRes; i++) {
        //cout << "String " << i << endl;
        for(int j = 0; j < xRes; j++) {
            Ray ray = MakeRay(j, i);
            //camera.pixels[i * xRes + j] = ray.dir;
            camera.pixels[i * xRes + j] = TraceRay(ray);
        }
    }
}

void
Tracer::SaveImageToFile(std::string fileName) const
{
    
    int width = camera.resolution.x;
    int height = camera.resolution.y;
    
    unsigned char *imageBuffer = new unsigned char[width * height * 24];
    
    int i, j;
    int imageDisplacement = 0;
    int textureDisplacement = 0;
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            glm::vec3 color = camera.pixels[textureDisplacement + j];
            
            imageBuffer[imageDisplacement + j * 3] =
                glm::clamp(color.r, 0.0f, 1.0f) * 255.0f;
            imageBuffer[imageDisplacement + j * 3 + 1] =
                glm::clamp(color.g, 0.0f, 1.0f) * 255.0f;
            imageBuffer[imageDisplacement + j * 3 + 2] =
                glm::clamp(color.b, 0.0f, 1.0f) * 255.0f;
        }
        
        imageDisplacement += width * 3;
        textureDisplacement += width;
    }
    
    lodepng_encode24_file(fileName.c_str(), imageBuffer, width, height);
}
