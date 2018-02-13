#include "Pch.h"

using namespace std;
using namespace glm;
using namespace Viper;
using namespace Viper::Asset;

namespace ModelPipeline
{
	MeshAsset* MeshProcessor::LoadMesh(aiMesh& mesh, const string& assetFullName)
	{
		auto meshAsset = new MeshAsset(StringID(assetFullName));
		auto& meshData = meshAsset->Data();

		// Vertices
		meshData.vertices.reserve(mesh.mNumVertices);
		for (uint32_t i = 0; i < mesh.mNumVertices; i++)
		{
			const auto& vertex = mesh.mVertices[i];
			meshData.vertices.push_back(vec3(vertex.x, vertex.y, vertex.z));
		}

		// Normals
		if (mesh.HasNormals())
		{
			meshData.normals.reserve(mesh.mNumVertices);
			for (uint32_t i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& normal = mesh.mNormals[i];
				meshData.normals.push_back(vec3(normal.x, normal.y, normal.z));
			}
		}

		// Texture coordinates (only for channel 0)
		meshData.textureCoordinates.reserve(mesh.mNumVertices);
		aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[0];
		for (uint32_t j = 0; j < mesh.mNumVertices; j++)
		{
			const auto& textureCoordinate = aiTextureCoordinates[j];
			meshData.textureCoordinates.push_back(vec3(textureCoordinate.x, textureCoordinate.y, textureCoordinate.z));
		}

		// Vertex Colors (only channel 0)
		meshData.vertexColors.reserve(mesh.mNumVertices);
		//aiColor4D* aiVertexColors = mesh.mColors[0];
		for (uint32_t j = 0; j < mesh.mNumVertices; j++)
		{
			//const auto& vertexColor = aiVertexColors[j];
			auto vertexColor = vec4();
			meshData.vertexColors.push_back(vec4(vertexColor.r, vertexColor.g, vertexColor.b, vertexColor.a));
		}

		// Faces (note: could pre-reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			meshData.faceCount = mesh.mNumFaces;
			for (uint32_t i = 0; i < meshData.faceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for (uint32_t j = 0; j < face->mNumIndices; j++)
				{
					meshData.indices.push_back(face->mIndices[j]);
				}
			}
		}

		// Tangents and Binormals
		if (mesh.HasTangentsAndBitangents())
		{
			meshData.tangents.reserve(mesh.mNumVertices);
			meshData.biNormals.reserve(mesh.mNumVertices);
			for (uint32_t i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& tangent = mesh.mTangents[i];
				const auto& biTangent = mesh.mBitangents[i];
				meshData.tangents.push_back(vec3(tangent.x, tangent.y, tangent.z));
				meshData.biNormals.push_back(vec3(biTangent.x, biTangent.y, biTangent.z));
			}
		}

		return meshAsset;
	}
}
