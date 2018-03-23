#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		Asset::Asset(const StringID& assetFullName, AssetType type) :
			assetManager(ServiceLocator::GetInstance().Get<AssetManager>()), assetId(assetFullName), type(type)
		{
		}

		const StringID& Asset::AssetId() const
		{
			return assetId;
		}

		AssetType Asset::Type() const
		{
			return type;
		}

		void Asset::Load()
		{
			throw GameException("This functionality is not implemented. Call load methods from asset manager.");
			// TODO: Implement move constructor for all asset types
			//auto assetPtr = assetManager.LoadSynchronous(assetId);
			//*this = *assetPtr;
		}

		void Asset::Save()
		{
			assetManager.SaveSynchronous(*this);
		}

		void Asset::RegisterAssetType(AssetType type, const AssetConstructor& constructor)
		{
			AssetManager::Constructors()[type] = constructor;
		}
	}

	OutputStreamHelper& operator<<(OutputStreamHelper& helper, const Assets::AssetType& value)
	{
		return helper << static_cast<uint8_t>(value);
	}

	InputStreamHelper& operator>>(InputStreamHelper& helper, Assets::AssetType& value)
	{
		uint8_t temp;
		helper >> temp;
		value = static_cast<Assets::AssetType>(temp);
		return helper;
	}
}
