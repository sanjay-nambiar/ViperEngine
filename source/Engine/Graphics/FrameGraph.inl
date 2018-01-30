
#ifndef FRAME_GRAPH_HEADER
#include "FrameGraph.h"
#endif

namespace Viper
{
	namespace Graphics
	{
		template <typename PassDataT>
		FrameGraphRenderPassNode& FrameGraph::AddRenderPass(const std::string& name, std::function<void(RenderPassBuilder&, PassDataT&)> setupCallback,
			std::function<void(const PassDataT&)> execCallback)
		{
			auto& output = builder.AddRenderPassNode(name);
			if (graphRoot == nullptr)
			{
				graphRoot = &output;
			}

			assert(builder.currentRenderPass != nullptr);
			renderPasses.push_back(builder.currentRenderPass);

			PassDataT data;
			setupCallback(builder, data);

			builder.currentRenderPass->renderCallback = [data, &execCallback]() {
				execCallback(data);
			};
			return output;
		}
	}
}
