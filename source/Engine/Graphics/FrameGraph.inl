
#ifndef FRAME_GRAPH_HEADER
#include "FrameGraph.h"
#endif

namespace Viper
{
	namespace Graphics
	{
		template <typename PassDataT>
		const std::vector<FrameGraphNode*>& FrameGraph::AddRenderPass(const std::string& name, std::function<void(RenderPassBuilder&, PassDataT&)> setupCallback,
			std::function<void(const PassDataT&, RendererSystem&)> execCallback)
		{
			auto& output = builder.AddRenderPassNode(name).next;
			assert(builder.currentRenderPass != nullptr);
			renderPasses.push_back(builder.currentRenderPass);

			PassDataT data;
			setupCallback(builder, data);
			builder.currentRenderPass = nullptr;
			
			renderCallback = [data, &rendererSystem]() {
				execCallback(data, rendererSystem)
			};
			return output;
		}
	}
}
