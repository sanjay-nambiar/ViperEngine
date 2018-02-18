#pragma once

#include <string>
#include "Asset/MeshAsset.h"

struct aiMesh;

namespace ModelPipeline
{
    class MeshProcessor
    {
    public:
		MeshProcessor() = delete;

		static Viper::Assets::MeshAsset* LoadMesh(aiMesh& mesh, const std::string& assetFullName);
    };
}