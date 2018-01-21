#pragma once
#include <vector>

namespace Viper
{
	namespace Graphics
	{
		class FrameGraphResourceNode;

		class FrameGraphRenderPassNode
		{
		public:
			FrameGraphRenderPassNode();
			~FrameGraphRenderPassNode();

		private:
			std::vector<FrameGraphResourceNode*> input;
			std::vector<FrameGraphResourceNode*> output;
		};
	}
}
