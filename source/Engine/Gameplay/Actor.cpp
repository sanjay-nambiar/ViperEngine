#include "Pch.h"
#include "Actor.h"
#include <cassert>
#include "Graphics/Mesh.h"

namespace Viper
{
	namespace Gameplay
	{
		Actor::Actor(Graphics::Mesh* actorMesh, const std::vector<Graphics::Texture*>& meshTexture) :
			mesh(actorMesh), textures(meshTexture)
		{
		}

		const Graphics::Mesh& Actor::Mesh() const
		{
			assert(mesh != nullptr);
			return *mesh;
		}

		const std::vector<Graphics::Texture*>& Actor::Textures() const
		{
			return textures;
		}

		glm::mat4& Actor::Transform()
		{
			return transform;
		}

		const glm::mat4& Actor::Transform() const
		{
			return transform;
		}
	}
}
