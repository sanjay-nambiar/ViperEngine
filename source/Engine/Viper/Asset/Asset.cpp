#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
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

		void Asset::Load()
		{
			auto& assetManager = ServiceLocator::GetInstance().Get<AssetManager>();
			auto& helper = assetManager.GetAssetInputStream(assetFullName);
			LoadFrom(helper);
		}

		void Asset::Save()
		{
			auto& assetManager = ServiceLocator::GetInstance().Get<AssetManager>();
			auto& helper = assetManager.GetAssetOutputStream(assetFullName);
			helper << static_cast<uint8_t>(type);
			SaveTo(helper);
		}

		void Asset::RegisterAssetType(AssetType type, const AssetConstructor& constructor)
		{
			AssetManager::Constructors[type] = constructor;
		}
	}
}
