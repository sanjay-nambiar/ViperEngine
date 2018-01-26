#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include "RTTI.h"
#include "RenderPassBuilder.h"
#include "Service/RendererSystem.h"

namespace Viper
{
	namespace Graphics
	{
		struct FrameGraphNode : public RTTI
		{
			FrameGraphNode();
			~FrameGraphNode() = default;

			std::vector<FrameGraphNode*> previous;
			std::vector<FrameGraphNode*> next;
			uint32_t nodeWeight;

			RTTI_DECLARATIONS(FrameGraphNode, RTTI)
		};

		struct FrameGraphRenderPassNode final : public FrameGraphNode
		{
			FrameGraphRenderPassNode(const std::string& name);

			std::string name;
			std::function<void()> renderCallback;

			RTTI_DECLARATIONS(FrameGraphRenderPassNode, FrameGraphNode)
		};

		struct FrameGraphResourceNode final : public FrameGraphNode
		{
			FrameGraphResourceNode(const TextureDescription& description);

			// compile time fields
			uint32_t resourceId;
			uint32_t resourceAlias;
			TextureDescription description;
			bool isResourceReady;

			// runtime time fields
			GpuTextureResource* gpuResource;

			RTTI_DECLARATIONS(FrameGraphResourceNode, FrameGraphNode)
		};

		class FrameGraph final
		{
		public:
			FrameGraph(ServiceLocator& serviceLocator);
			~FrameGraph();

			template <typename PassDataT>
			const std::vector<FrameGraphNode*>& AddRenderPass(const std::string& name, std::function<void(RenderPassBuilder&, PassDataT&)> setupCallback,
				std::function<void(const PassDataT&, RendererSystem&)> execCallback);

			void SetDisplayTarget(FrameGraphResourceNode& resource);

			void Compile();
			void AllocateGpuResources();
			void Render();
			void FreeGpuResources();

		private:
			friend class RenderPassBuilder;

			FrameGraphRenderPassNode* graphRoot;
			FrameGraphResourceNode* graphEnd;
			RenderPassBuilder builder;

			// TODO: change this to a reference and initialize this from service locator in constructor
			RendererSystem& rendererSystem;
			std::vector<FrameGraphRenderPassNode*> renderPasses;
			std::vector<FrameGraphRenderPassNode*>::iterator renderPassesIt;
			std::vector<FrameGraphResourceNode*> resources;
			std::vector<FrameGraphResourceNode*>::iterator resourcesIt;
			std::vector<GpuTextureResource*> gpuResources;
		};
	}
}

#define FRAME_GRAPH_HEADER

#include "FrameGraph.inl"
