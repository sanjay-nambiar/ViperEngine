#include "Viper.h"

using namespace std;

namespace Viper
{
	using namespace Memory;

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

		TextureAsset::~TextureAsset()
		{
			if (data.image.data != nullptr)
			{
				delete[] data.image.data;
			}
		}

		void TextureAsset::Load(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.width;
			inputHelper >> data.height;
			inputHelper >> data.channels;
			inputHelper >> data.isHdr;
			if (data.image.data != nullptr)
			{
				delete [] data.image.data;
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

		bool TextureAsset::operator==(const TextureAsset& rhs) const
		{
			if ((data.width != rhs.data.width) || (data.height != rhs.data.height) ||
				(data.channels != rhs.data.channels) || (data.isHdr != rhs.data.isHdr))
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
				if (data.image.data[i] != rhs.data.image.data[i])
				{
					return false;
				}
			}
			return true;
		}

		bool TextureAsset::operator!=(const TextureAsset& rhs) const
		{
			return !(*this == rhs);
		}

		TextureData& TextureAsset::Data()
		{
			return data;
		}
	}
}
