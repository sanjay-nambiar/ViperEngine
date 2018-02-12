#include "Viper.h"
#include "Asset.h"

using namespace std;

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

		void Asset::Load()
		{
			ifstream file(assetFullName.ToString(), ios::binary);
			if (!file.is_open())
			{
				throw GameException("Unable to load asset : " + assetFullName.ToString());
			}
			InputStreamHelper helper(file);
			Load(helper);
			file.close();
		}

		void Asset::Save()
		{
			ofstream file(assetFullName.ToString(), ios::binary);
			if (!file.is_open())
			{
				throw GameException("Unable to save asset : " + assetFullName.ToString());
			}
			OutputStreamHelper helper(file);
			Save(helper);
			file.close();
		}

		void Asset::SaveAs(const StringID& assetNewName)
		{
			assetFullName = assetNewName;
			Save();
		}
	}
}
