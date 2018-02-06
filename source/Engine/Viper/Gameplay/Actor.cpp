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

		glm::mat4& Actor::Model()
		{
			return model;
		}

		const glm::mat4& Actor::Model() const
		{
			return model;
		}

		glm::mat4& Actor::View()
		{
			return view;
		}

		const glm::mat4& Actor::View() const
		{
			return view;
		}

		glm::mat4& Actor::Projection()
		{
			return projection;
		}

		const glm::mat4& Actor::Projection() const
		{
			return projection;
		}
	}
}
