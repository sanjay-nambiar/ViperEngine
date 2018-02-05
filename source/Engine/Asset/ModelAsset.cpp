#include "Pch.h"
#include "ModelAsset.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		ModelAsset::ModelAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Model)
		{
		}

		const vector<pair<const MeshAsset*, const MaterialAsset*>>& ModelAsset::Meshes() const
		{
			return meshes;
		}
	}
}
