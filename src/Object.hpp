template<class ObjectMeshOperator>
typename std::result_of<ObjectMeshOperator(
    const LTriangle2 &,
    const LMaterial &,
    const Transform &
)>::type
Object::mesh_op(const ObjectMeshOperator &op) const
{  
    int meshes_c = l3dsObject->GetMeshCount();
    for (int i = 0; i < meshes_c; i++) {
        LMesh mesh = l3dsObject->GetMesh(i);
        int triangles_c = mesh.GetTriangleCount();
        for (int ii = 0; ii < triangles_c; ii++) {
            const LTriangle2 &triangle = mesh.GetTriangle2(ii);
            auto &material = l3dsObject->GetMaterial(triangle.materialId);
            auto op_res = op(triangle, material, transform);
            if (op_res != glm::vec3(0, 0, 0)) {
                return op_res;
            }
        }
    }
    
    //there was no positive result on any triangle:
    return glm::vec3(0, 0, 0);
}    

