#pragma once

#include <string>
#include "Asset/ModelAsset.h"

struct aiNode;

namespace ModelPipeline
{
    class ModelProcessor
    {
    public:
		ModelProcessor() = delete;

		static Viper::Assets::ModelAsset* LoadModel(const std::string& filename, bool flipUVs = false);
    };
}
