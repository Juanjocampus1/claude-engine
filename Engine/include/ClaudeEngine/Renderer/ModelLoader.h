#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "Model.h"
#include <string>

namespace ClaudeEngine {

    enum class ModelFormat {
        Unknown = 0,
        FBX,
        OBJ,
        GLTF,
        GLB
    };

    // Abstract model loader interface
    class IModelLoader {
    public:
        virtual ~IModelLoader() = default;
        virtual Ref<Model> Load(const std::string& filepath) = 0;
        virtual bool Supports(ModelFormat format) = 0;
    };

    // FBX Loader
    class FBXModelLoader : public IModelLoader {
    public:
        virtual Ref<Model> Load(const std::string& filepath) override;
        virtual bool Supports(ModelFormat format) override { return format == ModelFormat::FBX; }
    };

    // OBJ Loader
    class OBJModelLoader : public IModelLoader {
    public:
        virtual Ref<Model> Load(const std::string& filepath) override;
        virtual bool Supports(ModelFormat format) override { return format == ModelFormat::OBJ; }
    };

    // GLTF Loader
    class GLTFModelLoader : public IModelLoader {
    public:
        virtual Ref<Model> Load(const std::string& filepath) override;
        virtual bool Supports(ModelFormat format) override { 
            return format == ModelFormat::GLTF || format == ModelFormat::GLB; 
        }
    };

    // Model Loader Factory
    class ModelLoaderFactory {
    public:
        static ModelFormat DetectFormat(const std::string& filepath);
        static Ref<Model> LoadModel(const std::string& filepath);

    private:
        static Ref<IModelLoader> GetLoader(ModelFormat format);
    };

}
