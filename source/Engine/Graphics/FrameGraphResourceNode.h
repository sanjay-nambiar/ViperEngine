#pragma once
#include <vector>

namespace Viper
{
	namespace Graphics
	{
		class FrameGraphRenderPassNode;

		class FrameGraphResourceNode
		{
		public:
			FrameGraphResourceNode();
			~FrameGraphResourceNode();

		private:
			std::vector<FrameGraphRenderPassNode*> input;
			std::vector<FrameGraphRenderPassNode*> output;
		};
	}
}
