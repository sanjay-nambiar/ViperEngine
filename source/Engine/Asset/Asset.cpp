#include "Pch.h"
#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		Asset::Asset(const StringID& assetFullName, AssetType type) :
			assetFullName(assetFullName), type(type)
		{
		}

		const StringID& Asset::AssetFullName() const
		{
			return assetFullName;
		}

		AssetType Asset::Type() const
		{
			return type;
		}
	}
}
