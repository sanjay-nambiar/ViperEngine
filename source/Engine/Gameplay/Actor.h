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

			glm::mat4& Model();
			const glm::mat4& Model() const;

			glm::mat4& View();
			const glm::mat4& View() const;

			glm::mat4& Projection();
			const glm::mat4& Projection() const;
		private:
			Graphics::Mesh* mesh;
			std::vector<Graphics::Texture*> textures;
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
		};
	}
}
