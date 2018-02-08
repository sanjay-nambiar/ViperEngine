#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class MeshAsset : public Asset
		{
		public:
			MeshAsset(const StringID& assetFullName);
			~MeshAsset() = default;

			const std::vector<glm::vec3>& Vertices() const;
			const std::vector<glm::vec3>& Normals() const;
			const std::vector<glm::vec3>& Tangents() const;
			const std::vector<glm::vec3>& BiNormals() const;
			const std::vector<glm::vec2>& TextureCoordinates() const;
			const std::vector<glm::vec4>& VertexColors() const;
			const std::uint32_t FaceCount() const;
			const std::vector<std::uint32_t>& Indices() const;
		private:
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> tangents;
			std::vector<glm::vec3> biNormals;
			std::vector<glm::vec2> textureCoordinates;
			std::vector<glm::vec4> vertexColors;
			std::uint32_t faceCount;
			std::vector<std::uint32_t> indices;
		};
	}
}
