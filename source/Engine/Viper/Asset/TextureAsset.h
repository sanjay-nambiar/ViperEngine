#pragma once

#include "Asset.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Assets
	{
		class AssetManager;

		struct TextureData
		{
			TextureData();
			~TextureData() = default;

			std::uint32_t width;
			std::uint32_t height;
			std::uint32_t channels;
			bool isHdr;
			union
			{
				const uchar8_t* data;
				const float32_t* hdrData;
			} image;
		};

		class TextureAsset : public Asset
		{
		public:
			ASSET_DECLARATION(TextureAsset)
			~TextureAsset();

			TextureData& Data();

			bool operator==(const TextureAsset& rhs) const;
			bool operator!=(const TextureAsset& rhs) const;
		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;

			TextureData data;
		};
	}
}
