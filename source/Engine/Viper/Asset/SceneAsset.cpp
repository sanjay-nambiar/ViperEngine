#include "Viper.h"
#include "SceneAsset.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		SceneAsset::SceneAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Scene)
		{
		}

		void SceneAsset::Load(InputStreamHelper&)
		{
		}

		void SceneAsset::Save(OutputStreamHelper&) const
		{
		}
	}
}
