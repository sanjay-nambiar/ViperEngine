#include "Viper.h"
#include "SceneAsset.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		ASSET_DEFINITION(SceneAsset, Asset, AssetType::Scene)

		void SceneAsset::LoadFrom(InputStreamHelper&)
		{
		}

		void SceneAsset::SaveTo(OutputStreamHelper&) const
		{
		}

		bool SceneAsset::operator==(const Asset&) const
		{
			return true;
		}

		bool SceneAsset::operator!=(const Asset& rhs) const
		{
			return !(*this == rhs);
		}
	}
}
