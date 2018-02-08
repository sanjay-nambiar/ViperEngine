#pragma once

#include "Core/Types.h"

namespace Viper
{
	namespace Graphics
	{
		class Mesh final
		{
		public:
			Mesh(std::uint32_t verticesCount, float32_t vertices[], std::uint32_t triangleCount, std::uint32_t tris[]);
			~Mesh();

			std::uint32_t VertexCount() const;
			const float32_t* VertexData() const;
			std::uint32_t VertexDataSize() const;
			std::uint32_t TriCount() const;
			const std::uint32_t* TriData() const;
			std::uint32_t TriDataSize() const;
		private:
			float32_t* vertexData;
			std::uint32_t vertexCount;
			std::uint32_t* triData;
			std::uint32_t triCount;
		};
	}
}
