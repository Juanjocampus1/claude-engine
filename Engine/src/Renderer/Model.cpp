#include "ClaudeEngine/Renderer/Model.h"
#include "ClaudeEngine/Core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ClaudeEngine {

    Model::Model(const std::string& path) {
        LoadModel(path);
    }

    void Model::Draw(const Ref<Shader>& shader) {
        for (auto& mesh : m_Meshes) {
            mesh->Draw(shader);
        }
    }

    void Model::LoadModel(const std::string& path) {
        CE_CORE_INFO("Loading model: ", path);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate | 
            aiProcess_FlipUVs | 
            aiProcess_CalcTangentSpace |
            aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            CE_CORE_ERROR("Assimp error: ", importer.GetErrorString());
            return;
        }

        m_Directory = path.substr(0, path.find_last_of('/'));

        // Process nodes recursively
        std::function<void(aiNode*, const aiScene*)> processNode = [&](aiNode* node, const aiScene* scene) {
            // Process all meshes
            for (uint32_t i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                
                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;
                std::vector<MeshTexture> textures;

                // Vertices
                for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
                    Vertex vertex;
                    vertex.Position = { mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z };
                    
                    if (mesh->HasNormals())
                        vertex.Normal = { mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z };
                    
                    if (mesh->mTextureCoords[0])
                        vertex.TexCoords = { mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y };
                    else
                        vertex.TexCoords = { 0.0f, 0.0f };

                    if (mesh->HasTangentsAndBitangents()) {
                        vertex.Tangent = { mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z };
                        vertex.Bitangent = { mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z };
                    }

                    vertices.push_back(vertex);
                }

                // Indices
                for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
                    aiFace face = mesh->mFaces[j];
                    for (uint32_t k = 0; k < face.mNumIndices; k++)
                        indices.push_back(face.mIndices[k]);
                }

                // Materials/Textures (simplified for now)
                if (mesh->mMaterialIndex >= 0) {
                    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                    // Load textures here if needed
                }

                m_Meshes.push_back(CreateRef<Mesh>(vertices, indices, textures));
            }

            // Process children
            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }
        };

        processNode(scene->mRootNode, scene);
        CE_CORE_INFO("Model loaded successfully: ", m_Meshes.size(), " meshes");
    }

}
