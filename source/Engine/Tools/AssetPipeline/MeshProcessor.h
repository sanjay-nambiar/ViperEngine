#pragma once

#include <string>
#include "AssetProcessor.h"
#include "Asset/MeshAsset.h"
#include "Core/Types.h"

struct aiMesh;

namespace AssetPipeline
{
    class MeshProcessor
    {
    public:
		MeshProcessor(AssetProcessor& assetProcessor);
		MeshProcessor(const MeshProcessor&) = delete;
		MeshProcessor(MeshProcessor&&) = delete;

		Viper::Assets::MeshAsset* LoadMesh(const Resource& resource, aiMesh& mesh, std::uint32_t index = 0);

	private:
		AssetProcessor & assetProcessor;
    };
}