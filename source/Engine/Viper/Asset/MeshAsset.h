#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Asset.h"

namespace Viper
{
	namespace Assets
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
			ASSET_DECLARATION(MeshAsset)
			~MeshAsset() = default;

			MeshData& Data();

			bool operator==(const Asset& rhs) const;
			bool operator!=(const Asset& rhs) const;
		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;

			MeshData data;
		};
	}
}
