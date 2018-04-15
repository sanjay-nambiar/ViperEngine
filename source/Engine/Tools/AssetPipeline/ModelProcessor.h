#pragma once

#include <string>
#include "Asset/ModelAsset.h"
#include "AssetProcessor.h"
#include "Core/Types.h"

struct aiNode;

namespace AssetPipeline
{
	class MeshProcessor;
	class MaterialProcessor;

    class ModelProcessor
    {
    public:
		ModelProcessor(AssetProcessor& assetProcessor, MeshProcessor& meshProcessor, MaterialProcessor& materialProcessor);
		ModelProcessor(const ModelProcessor&) = delete;
		ModelProcessor(ModelProcessor&&) = delete;

		Viper::Assets::ModelAsset* LoadModel(const Resource& resource, std::uint32_t index = 0, bool flipUVs = false);

	private:
		AssetProcessor& assetProcessor;
		MeshProcessor& meshProcessor;
		MaterialProcessor& materialProcessor;
    };
}
