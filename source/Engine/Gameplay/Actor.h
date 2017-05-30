#pragma once

#include "glm.hpp"

namespace Viper
{
	namespace Graphics
	{
		class Mesh;
		class Texture;
	}

	namespace Gameplay
	{
		class Actor
		{
		public:
			Actor(Graphics::Mesh* mesh, const std::vector<Graphics::Texture*>& textures);

			const Graphics::Mesh& Mesh() const;
			const std::vector<Graphics::Texture*>& Textures() const;
			glm::mat4& Transform();
			const glm::mat4& Transform() const;
		private:
			Graphics::Mesh* mesh;
			std::vector<Graphics::Texture*> textures;
			glm::mat4 transform;
		};
	}
}
