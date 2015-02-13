#include "Object.h"

using namespace std;


map<const char *, shared_ptr<L3DS>> Object::l3dsObjectsDict;

Object::Object(
    const char *fName,
    glm::mat4x4 translate,
    glm::mat4x4 rotate,
    glm::mat4x4 scale
) :
    l3dsObject(nullptr),
    transform(translate, rotate, scale)
{
    auto l3dsObjectsDict_iter = l3dsObjectsDict.find(fName);
    if (l3dsObjectsDict_iter != l3dsObjectsDict.end()) {
        //existing object
        l3dsObject = l3dsObjectsDict_iter->second;
    } else {
        //new object
        l3dsObject = shared_ptr<L3DS>(new L3DS(fName));
        l3dsObjectsDict.emplace(fName, l3dsObject);
    }
    
    glm::vec3 center = centerOfMass();
    boundingSphere = make_tuple(center, longestDistance(center));
}

glm::vec3
Object::centerOfMass() const
{
    int meshes_c = l3dsObject->GetMeshCount(), total_triangles_c = 0;
    auto sum = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < meshes_c; i++) {
        LMesh mesh = l3dsObject->GetMesh(i);
        int triangles_c = mesh.GetTriangleCount();
        total_triangles_c += triangles_c;
        for (int ii = 0; ii < triangles_c; ii++) {
            const auto &vertices = mesh.GetTriangle2(ii).vertices;
            glm::vec3 triangle_cen =
                (lvec2glmvec(vertices[0]) + lvec2glmvec(vertices[1]) + lvec2glmvec(vertices[2])) / 3.0f;
            sum += triangle_cen;
        }
    }
    return sum / float(total_triangles_c);
}

float
Object::longestDistance(glm::vec3 center) const
{
    int meshes_c = l3dsObject->GetMeshCount();
    float max_dist = 0.0f, dist[3];
    for (int i = 0; i < meshes_c; i++) {
        LMesh mesh = l3dsObject->GetMesh(i);
        int triangles_c = mesh.GetTriangleCount();
        for (int ii = 0; ii < triangles_c; ii++) {
            const auto &vertices = mesh.GetTriangle2(ii).vertices;
            dist[0] = glm::distance(center, lvec2glmvec(vertices[0]));
            dist[1] = glm::distance(center, lvec2glmvec(vertices[1]));
            auto cur_max = std::max(dist[0], dist[1]);
            dist[2] = glm::distance(center, lvec2glmvec(vertices[2]));
            cur_max = std::max(dist[1], dist[2]);
            if (cur_max > max_dist) {
                max_dist = cur_max;
            }
        }
    }
    return max_dist;
}

const Transform &
Object::GetTransform() const
{
    return transform;
}

tuple<glm::vec3, float>
Object::GetBoundingSphere() const
{
    return boundingSphere;
}
