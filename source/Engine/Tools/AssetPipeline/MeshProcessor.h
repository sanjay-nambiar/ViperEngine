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

		static Viper::Asset::MeshAsset* LoadMesh(aiMesh& mesh, const std::string& assetFullName);
    };
}