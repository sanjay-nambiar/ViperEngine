#include "Viper.h"

using namespace std;

namespace Viper
{
	using namespace Memory;

	namespace Assets
	{
		TextureData::TextureData() :
			width(0), height(0), channels(0), isHdr(false), image({ nullptr })
		{
		}

		ASSET_DEFINITION(TextureAsset, Asset, AssetType::Texture)

		TextureAsset::~TextureAsset()
		{
			if (data.image.data != nullptr)
			{
				delete[] data.image.data;
			}
		}

		bool TextureAsset::operator==(const Asset& rhs) const
		{
			if (type != rhs.Type())
			{
				return false;
			}
			const auto& rhsTexture = static_cast<const TextureAsset&>(rhs);
			if ((data.width != rhsTexture.data.width) || (data.height != rhsTexture.data.height) ||
				(data.channels != rhsTexture.data.channels) || (data.isHdr != rhsTexture.data.isHdr))
			{
				return false;
			}

			uint32_t totalComponents = data.channels * data.width * data.height;
			if (data.isHdr)
			{
				totalComponents *= sizeof(float32_t);
			}

			for (uint32_t i = 0; i < totalComponents; ++i)
			{
				if (data.image.data[i] != rhsTexture.data.image.data[i])
				{
					return false;
				}
			}
			return true;
		}

		bool TextureAsset::operator!=(const Asset& rhs) const
		{
			return !(*this == rhs);
		}

		TextureData& TextureAsset::Data()
		{
			return data;
		}

		void TextureAsset::LoadFrom(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.width;
			inputHelper >> data.height;
			inputHelper >> data.channels;
			inputHelper >> data.isHdr;
			if (data.image.data != nullptr)
			{
				delete[] data.image.data;
			}

			auto& inputStream = inputHelper.Stream();
			if (data.isHdr)
			{
				data.image.hdrData = new float32_t[data.channels * data.width * data.height];
				auto buffer = const_cast<char8_t*>(reinterpret_cast<const char8_t*>(data.image.hdrData));
				inputStream.read(buffer, data.channels * data.width * data.height * sizeof(float32_t));
			}
			else
			{
				data.image.data = new uchar8_t[data.channels * data.width * data.height];
				auto buffer = const_cast<char8_t*>(reinterpret_cast<const char8_t*>(data.image.data));
				inputStream.read(buffer, data.channels * data.width * data.height * sizeof(uchar8_t));
			}
		}

		void TextureAsset::SaveTo(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.width;
			outputHelper << data.height;
			outputHelper << data.channels;
			outputHelper << data.isHdr;
			auto& ouputStream = outputHelper.Stream();
			if (data.isHdr)
			{
				ouputStream.write(reinterpret_cast<const char8_t*>(data.image.hdrData),
					data.channels * data.width * data.height * sizeof(float32_t));
			}
			else
			{
				ouputStream.write(reinterpret_cast<const char8_t*>(data.image.data),
					data.channels * data.width * data.height * sizeof(uchar8_t));
			}
		}
	}
}
