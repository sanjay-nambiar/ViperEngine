#include "Viper.h"
#include "SceneAsset.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		SceneAsset::SceneAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Scene)
		{
		}

		void SceneAsset::LoadFrom(InputStreamHelper&)
		{
		}

		void SceneAsset::SaveTo(OutputStreamHelper&) const
		{
		}
	}
}
