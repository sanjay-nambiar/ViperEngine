#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class MaterialAsset;

		struct MeshData
		{
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> tangents;
			std::vector<glm::vec3> biNormals;
			std::vector<glm::vec3> textureCoordinates;
			std::vector<glm::vec4> vertexColors;
			std::uint32_t faceCount;
			std::vector<std::uint32_t> indices;
		};

		class MeshAsset : public Asset
		{
		public:
			MeshAsset(const StringID& assetFullName);
			~MeshAsset() = default;

			MeshData& Data();

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;

			bool operator==(const MeshAsset& rhs) const;
			bool operator!=(const MeshAsset& rhs) const;
		private:
			MeshData data;
		};
	}
}
