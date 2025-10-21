#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "Mesh.h"
#include <string>
#include <vector>

namespace ClaudeEngine {

    class Model {
    public:
        Model() = default;
        Model(const std::string& path);
        ~Model() = default;

        void Draw(const Ref<Shader>& shader);
        void AddMesh(const Ref<Mesh>& mesh) { m_Meshes.push_back(mesh); }

        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    private:
        void LoadModel(const std::string& path);

    private:
        std::vector<Ref<Mesh>> m_Meshes;
        std::string m_Directory;
    };

}
