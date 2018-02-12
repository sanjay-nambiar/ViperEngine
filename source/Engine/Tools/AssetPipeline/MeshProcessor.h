#pragma once

#include "Asset/MeshAsset.h"

struct aiMesh;

namespace ModelPipeline
{
    class MeshProcessor
    {
    public:
		MeshProcessor() = delete;

		static Viper::Asset::MeshAsset* LoadMesh(Viper::Asset::ModelData& model, aiMesh& mesh);
    };
}