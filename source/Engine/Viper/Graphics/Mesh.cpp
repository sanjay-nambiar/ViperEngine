#include "Pch.h"
#include "Mesh.h"

namespace Viper
{
	namespace Graphics
	{
		#define FLOATS_PER_VERTEX 8	// 3D coordinates + RGB colors + UV

		Mesh::Mesh(std::uint32_t verticesCount, float32_t vertices[], std::uint32_t triangleCount, std::uint32_t tris[])
		{
			vertexCount = verticesCount;
			vertexData = new float32_t[vertexCount * FLOATS_PER_VERTEX];
			triCount = triangleCount;
			triData = new std::uint32_t[triCount * 3];
			memcpy(vertexData, vertices, sizeof(float32_t) * vertexCount * FLOATS_PER_VERTEX);
			memcpy(triData, tris, sizeof(std::uint32_t) * triCount * 3);
		}

		Mesh::~Mesh()
		{
			delete[] vertexData;
			delete[] triData;
		}

		std::uint32_t Mesh::VertexCount() const
		{
			return vertexCount;
		}

		const float32_t* Mesh::VertexData() const
		{
			return vertexData;
		}

		std::uint32_t Mesh::VertexDataSize() const
		{
			return (sizeof(float32_t) * vertexCount * FLOATS_PER_VERTEX);
		}

		std::uint32_t Mesh::TriCount() const
		{
			return triCount;
		}

		const std::uint32_t* Mesh::TriData() const
		{
			return triData;
		}

		std::uint32_t Mesh::TriDataSize() const
		{
			return (sizeof(std::uint32_t) * triCount * 3);
		}
	}
}
