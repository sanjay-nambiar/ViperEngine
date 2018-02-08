#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "Core/ServiceLocator.h"
#include "Core/Types.h"
#include "Renderer.h"
#include "RenderPassBuilder.h"

namespace Viper
{
	namespace Graphics
	{
		struct FrameGraphNode
		{
			FrameGraphNode(const std::string& name);
			~FrameGraphNode() = default;

			const std::string name;
			std::vector<FrameGraphNode*> previous;
			std::vector<FrameGraphNode*> next;
			uint32_t nodeWeight;
		};

		struct FrameGraphRenderPassNode final : public FrameGraphNode
		{
			FrameGraphRenderPassNode(const std::string& name);

			std::function<void()> renderCallback;
		};

		struct FrameGraphResourceNode final : public FrameGraphNode
		{
			FrameGraphResourceNode(const std::string& name, const TextureDescription& description);

			// compile time fields
			uint32_t resourceId;
			uint32_t resourceAlias;
			const TextureDescription description;
			bool isResourceReady;

			// runtime time fields
			GpuTextureResource* gpuResource;
		};

		class FrameGraph final
		{
		public:
			FrameGraph(ServiceLocator& serviceLocator);
			~FrameGraph();

			template <typename PassDataT>
			FrameGraphRenderPassNode& AddRenderPass(const std::string& name, std::function<void(RenderPassBuilder&, PassDataT&)> setupCallback,
				std::function<void(const PassDataT&)> execCallback);

			void SetDisplayTarget(FrameGraphResourceNode& resource);

			void Compile();
			void AllocateGpuResources();
			void Render();
			void FreeGpuResources();
			void DebugOutput(const std::string& graphName, const std::string& filename);

		private:
			friend class RenderPassBuilder;

			FrameGraphRenderPassNode* graphRoot;
			FrameGraphNode* graphEnd;
			RenderPassBuilder builder;

			// TODO: change this to a reference and initialize this from service locator in constructor
			Renderer& rendererSystem;
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
