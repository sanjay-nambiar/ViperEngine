#pragma once

#include "Service/RendererSystem.h"

struct GLFWwindow;

namespace Viper
{
	namespace Renderer
	{
		class OpenGLRenderer final : public RendererSystem
		{
		public:
			OpenGLRenderer() = default;
			~OpenGLRenderer() = default;

			void Initialize() override;
			void Update() override;
			void Shutdown() override;
		};
	}
}
