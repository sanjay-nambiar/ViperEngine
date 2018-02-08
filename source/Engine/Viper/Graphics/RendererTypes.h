#pragma once

#include "Core/Types.h"

namespace Viper
{
	namespace Graphics
	{
		enum class TextureType
		{
			TEXTURE_1D,
			TEXTURE_2D,
			TEXTURE_3D,
			TEXTURE_CUBE_MAP
		};

		enum class TextureFormat
		{
			D16,
			D24,
			D32F,
			D24S8,
			D32FS8,
			R16F,
			RG16F,
			RGB16F,
			RGBA16F,
			R32F,
			RG32F,
			RGB32F,
			RGBA32F,
			R8I,
			R16I,
			R32I,
			RG8I,
			RG16I,
			RG32I,
			RGB8I,
			RGB16I,
			RGB32I,
			RGBA8I,
			RGBA16I,
			RGBA32I,
			R8UI,
			R16UI,
			R32UI,
			RG8UI,
			RG16UI,
			RG32UI,
			RGB8UI,
			RGB16UI,
			RGB32UI,
			RGBA8UI,
			RGBA16UI,
			RGBA32UI
		};

		enum class TextureWrapping
		{
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			REPEAT,
			MIRRORED_REPEAT,
			MIRROR_CLAMP_TO_EDGE
		};

		enum class TextureFiltering
		{
			NEAREST,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		struct TextureDescription
		{
			TextureType type;
			TextureFormat format;
			std::uint32_t width;
			std::uint32_t height;
			void* data;
			TextureWrapping wrapS;
			TextureWrapping wrapT;
			TextureFiltering minFilter;
			TextureFiltering magFilter;
		};

		struct GpuTextureResource { };

		enum class Primitive
		{
			Point,
			Line,
			Triangle
		};
	}
}