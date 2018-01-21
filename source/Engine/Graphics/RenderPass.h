#pragma once

#include <vector>
#include "RenderingPrimitive.h"

namespace Viper
{
	namespace Graphics
	{
		class BlackBoard;

		class RenderPass
		{
		public:
			RenderPass();
			~RenderPass();
			void Render(std::vector<std::vector<RenderingPrimitive>> primtiives, BlackBoard& blackboard);
		};
	}
}
