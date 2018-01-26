#pragma once

#include <vector>
#include "RenderPrimitive.h"

namespace Viper
{
	namespace Graphics
	{
		class BlackBoard;

		class RenderPass
		{
		public:
			RenderPass() = default;
			virtual ~RenderPass() = default;
			virtual void Render(std::vector<std::vector<RenderPrimitive>> primtiives, BlackBoard& blackboard) = 0;
		};
	}
}
