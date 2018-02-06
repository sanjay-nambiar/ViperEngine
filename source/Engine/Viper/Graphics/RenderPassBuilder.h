#pragma once

namespace Viper
{
	namespace Graphics
	{
		class FrameGraph;
		struct FrameGraphNode;
		struct FrameGraphResourceNode;
		struct FrameGraphRenderPassNode;
		struct TextureDescription;

		class RenderPassBuilder final
		{
		public:
			FrameGraphResourceNode& Read(FrameGraphResourceNode& input);
			FrameGraphResourceNode& Write(const std::string& name, FrameGraphResourceNode& output);
			FrameGraphResourceNode& CreateWrite(const std::string& newName, const TextureDescription& description);

		private:
			friend class FrameGraph;

			RenderPassBuilder(FrameGraph& frameGraph);
			~RenderPassBuilder() = default;

			FrameGraphRenderPassNode& AddRenderPassNode(const std::string& name);

			FrameGraph& frameGraph;
			FrameGraphRenderPassNode* currentRenderPass;
			uint32_t resourceId;
		};
	}
}
