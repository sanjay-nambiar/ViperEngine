#pragma once

#include <cstdint>

namespace Viper
{
	class RendererSystem
	{
	public:
		RendererSystem() = default;
		virtual ~RendererSystem() = default;

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}
