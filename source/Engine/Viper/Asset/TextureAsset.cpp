#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		TextureData::TextureData() :
			width(0), height(0), channels(0), isHdr(false), image({ nullptr })
		{
		}

		TextureAsset::TextureAsset(const StringID& assetFullName) :
			Asset(assetFullName)
		{
		}

		void TextureAsset::Load(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.width;
			inputHelper >> data.height;
			inputHelper >> data.channels;
			inputHelper >> data.isHdr;
			auto& inputStream = inputHelper.Stream();
			if (data.isHdr)
			{
				auto buffer = const_cast<char8_t*>(reinterpret_cast<const char8_t*>(data.image.hdrData));
				inputStream.read(buffer, data.channels * data.width * data.height * sizeof(float32_t));
			}
			else
			{
				auto buffer = const_cast<char8_t*>(reinterpret_cast<const char8_t*>(data.image.data));
				inputStream.read(buffer, data.channels * data.width * data.height * sizeof(uchar8_t));
			}
		}

		void TextureAsset::Save(OutputStreamHelper& outputHelper) const
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

		TextureData& TextureAsset::Data()
		{
			return data;
		}
	}
}
