#pragma once
#include "glm/glm.hpp"

namespace Viper
{
	namespace Graphics
	{
		struct Blackboard final
		{
		public:
			Blackboard() = default;
			~Blackboard() = default;

			glm::mat4 cameraView;
			glm::mat4 cameraProjection;
			glm::vec3 cameraPosition;
			glm::vec3 cameraUp;
			glm::vec3 cameraForward;
		};
	}
}
