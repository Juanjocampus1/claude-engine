#include "ClaudeEngine/Renderer/ModelLoader.h"
#include "ClaudeEngine/Core/Log.h"
#include <algorithm>

namespace ClaudeEngine {

    // ========== FBX Loader ==========
    Ref<Model> FBXModelLoader::Load(const std::string& filepath) {
        CE_CORE_INFO("Loading FBX model: ", filepath);
        // Use Assimp which supports FBX
        return CreateRef<Model>(filepath);
    }

    // ========== OBJ Loader ==========
    Ref<Model> OBJModelLoader::Load(const std::string& filepath) {
        CE_CORE_INFO("Loading OBJ model: ", filepath);
        // Use Assimp which supports OBJ
        return CreateRef<Model>(filepath);
    }

    // ========== GLTF Loader ==========
    Ref<Model> GLTFModelLoader::Load(const std::string& filepath) {
        CE_CORE_INFO("Loading GLTF model: ", filepath);
        // Use Assimp which supports GLTF
        return CreateRef<Model>(filepath);
    }

    // ========== Model Loader Factory ==========

    ModelFormat ModelLoaderFactory::DetectFormat(const std::string& filepath) {
        std::string extension = filepath.substr(filepath.find_last_of(".") + 1);
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if (extension == "fbx") return ModelFormat::FBX;
        if (extension == "obj") return ModelFormat::OBJ;
        if (extension == "gltf") return ModelFormat::GLTF;
        if (extension == "glb") return ModelFormat::GLB;

        CE_CORE_WARN("Unknown model format: ", extension);
        return ModelFormat::Unknown;
    }

    Ref<IModelLoader> ModelLoaderFactory::GetLoader(ModelFormat format) {
        switch (format) {
            case ModelFormat::FBX:  return CreateRef<FBXModelLoader>();
            case ModelFormat::OBJ:  return CreateRef<OBJModelLoader>();
            case ModelFormat::GLTF:
            case ModelFormat::GLB:  return CreateRef<GLTFModelLoader>();
            default:
                CE_CORE_ERROR("Unsupported model format!");
                return nullptr;
        }
    }

    Ref<Model> ModelLoaderFactory::LoadModel(const std::string& filepath) {
        ModelFormat format = DetectFormat(filepath);
        if (format == ModelFormat::Unknown) {
            CE_CORE_ERROR("Cannot load model with unknown format: ", filepath);
            return nullptr;
        }

        auto loader = GetLoader(format);
        if (!loader) {
            CE_CORE_ERROR("No loader available for format");
            return nullptr;
        }

        return loader->Load(filepath);
    }

}
