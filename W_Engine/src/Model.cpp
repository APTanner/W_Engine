#include <W_Engine/Model.h>

namespace W_Engine
{
    void Model::Draw(const Transform& transform, Shader& shader) const
    {
        for (const std::unique_ptr<Mesh>& mesh : Meshes)
        {
            mesh.get()->Draw(transform, shader);
        }
    }
}