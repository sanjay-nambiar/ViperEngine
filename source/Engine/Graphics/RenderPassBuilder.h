#pragma once

namespace Viper
{
	struct TextureDescription;

	namespace Graphics
	{
		class FrameGraph;
		struct FrameGraphNode;
		struct FrameGraphResourceNode;
		struct FrameGraphRenderPassNode;

		class RenderPassBuilder final
		{
		public:
			void Read(FrameGraphResourceNode& input);
			void Write(FrameGraphResourceNode& output);
			void CreateWrite(const TextureDescription& description);

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
