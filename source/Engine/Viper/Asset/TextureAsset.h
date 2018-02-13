#pragma once

#include "Asset.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Asset
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
			TextureAsset(const StringID& assetFullName);
			~TextureAsset() = default;

			TextureData& Data();

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;

			bool operator==(const TextureAsset& rhs) const;
			bool operator!=(const TextureAsset& rhs) const;
		private:
			TextureData data;
		};
	}
}
