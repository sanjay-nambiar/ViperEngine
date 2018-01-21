#pragma once
#include "glm/glm.hpp"

namespace Viper
{
	namespace Graphics
	{
		class Blackboard
		{
		public:
			Blackboard();
			~Blackboard();

			glm::mat4 cameraView;
			glm::mat4 cameraProjection;
			glm::vec3 cameraPosition;
			glm::vec3 cameraUp;
			glm::vec3 cameraForward;
		};
	}
}
