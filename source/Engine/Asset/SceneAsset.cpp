#include "Pch.h"
#include "SceneAsset.h"

namespace Viper
{
	namespace Asset
	{
		SceneAsset::SceneAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Scene)
		{
		}
	}
}
