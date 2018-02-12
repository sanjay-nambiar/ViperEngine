#include "Viper.h"
#include "MeshAsset.h"

using namespace std;
using namespace glm;

namespace Viper
{
	namespace Asset
	{
		MeshAsset::MeshAsset(const StringID& assetFullName) :
			Asset(assetFullName, AssetType::Mesh)
		{
		}

		void MeshAsset::Load(InputStreamHelper& inputHelper)
		{
			uint32_t tempCount, temp;
			glm::vec3 tempVec3;
			glm::vec4 tempVec4;

			inputHelper >> tempCount;
			data.vertices.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec3;
				data.vertices.push_back(tempVec3);
			}
			inputHelper >> tempCount;
			data.normals.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec3;
				data.normals.push_back(tempVec3);
			}
			inputHelper >> tempCount;
			data.tangents.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec3;
				data.tangents.push_back(tempVec3);
			}
			inputHelper >> tempCount;
			data.biNormals.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec3;
				data.biNormals.push_back(tempVec3);
			}
			inputHelper >> tempCount;
			data.textureCoordinates.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec3;
				data.textureCoordinates.push_back(tempVec3);
			}
			inputHelper >> tempCount;
			data.vertexColors.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> tempVec4;
				data.vertexColors.push_back(tempVec4);
			}

			inputHelper >> data.faceCount;

			inputHelper >> tempCount;
			data.indices.reserve(tempCount);
			for (uint32_t i = 0; i < tempCount; ++i)
			{
				inputHelper >> temp;
				data.indices.push_back(temp);
			}
		}

		void MeshAsset::Save(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.vertices.size();
			for (auto& vertex : data.vertices)
			{
				outputHelper << vertex;
			}
			outputHelper << data.normals.size();
			for (auto& normal : data.normals)
			{
				outputHelper << normal;
			}
			outputHelper << data.tangents.size();
			for (auto& tangent : data.tangents)
			{
				outputHelper << tangent;
			}
			outputHelper << data.biNormals.size();
			for (auto& biNormal : data.biNormals)
			{
				outputHelper << biNormal;
			}
			outputHelper << data.textureCoordinates.size();
			for (auto& textureCoordinate : data.textureCoordinates)
			{
				outputHelper << textureCoordinate;
			}
			outputHelper << data.vertexColors.size();
			for (auto& vertexColor : data.vertexColors)
			{
				outputHelper << vertexColor;
			}

			outputHelper << data.faceCount;

			outputHelper << data.indices.size();
			for (auto& indice : data.indices)
			{
				outputHelper << indice;
			}
		}

		MeshData& MeshAsset::Data()
		{
			return data;
		}
	}
}
