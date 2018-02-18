#include "Viper.h"
#include "AssetManager.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		AssetManager::AssetManager() :
			Service(ServiceType::AssetManager)
		{
		}

		void AssetManager::LoadSynchronous(const StringID&)
		{

		}

		void AssetManager::UnloadSynchronous(const StringID&)
		{

		}

		void AssetManager::UnloadAll()
		{

		}

		vector<Asset> AssetManager::Search(const StringID&)
		{
			return vector<Asset>();
		}

		InputStreamHelper& AssetManager::GetAssetInputStream(StringID)
		{
			// TODO: Lookup asset registry and find appropriate file / memory stream for read
			istream stream(nullptr);
			InputStreamHelper* helper = new InputStreamHelper(stream);
			return *helper;
		}

		OutputStreamHelper& AssetManager::GetAssetOutputStream(StringID)
		{
			// TODO: Lookup asset registry and find appropriate file / memory stream for write
			ostream stream(nullptr);
			OutputStreamHelper* helper = new OutputStreamHelper(stream);
			return *helper;
		}
	}
}
