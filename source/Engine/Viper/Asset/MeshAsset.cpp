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

		const vector<vec3>& MeshAsset::Vertices() const
		{
			return vertices;
		}

		const vector<vec3>& MeshAsset::Normals() const
		{
			return normals;
		}

		const vector<vec3>& MeshAsset::Tangents() const
		{
			return tangents;
		}

		const vector<vec3>& MeshAsset::BiNormals() const
		{
			return biNormals;
		}
		
		const vector<vec2>& MeshAsset::TextureCoordinates() const
		{
			return textureCoordinates;
		}
		
		const vector<vec4>& MeshAsset::VertexColors() const
		{
			return vertexColors;
		}
		
		const uint32_t MeshAsset::FaceCount() const
		{
			return faceCount;
		}
		
		const vector<uint32_t>& MeshAsset::Indices() const
		{
			return indices;
		}
	}
}
