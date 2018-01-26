#include "Pch.h"
#include "FrameGraph.h"

using namespace std;

namespace Viper
{
	namespace Graphics
	{
		FrameGraphNode::FrameGraphNode()
			: nodeWeight(0)
		{
		}


		FrameGraphRenderPassNode::FrameGraphRenderPassNode(const string& name)
			: name(name)
		{
		}



		FrameGraphResourceNode::FrameGraphResourceNode(const TextureDescription& description)
			: resourceId(0), resourceAlias(0), description(description), isResourceReady(false), gpuResource(nullptr)
		{
		}


		FrameGraph::FrameGraph(ServiceLocator& serviceLocator)
			: graphRoot(nullptr), graphEnd(nullptr), builder(*this), rendererSystem(serviceLocator.GetRendererSystem())
		{
		}

		FrameGraph::~FrameGraph()
		{
			FreeGpuResources();

			for (auto renderPass : renderPasses)
			{
				delete renderPass;
			}

			for (auto resource : resources)
			{
				delete resource;
			}
		}

		void FrameGraph::SetDisplayTarget(FrameGraphResourceNode& resource)
		{
			graphEnd = &resource;
		}

		void FrameGraph::Compile()
		{
			queue<FrameGraphNode*> reverseQueue;
			graphEnd->nodeWeight = 0;
			reverseQueue.push(graphEnd);

			while (!reverseQueue.empty())
			{
				FrameGraphNode* node = reverseQueue.front();
				reverseQueue.pop();

				uint32_t newNodeWeight = node->nodeWeight + 1;
				for (auto prev : node->previous)
				{
					if (newNodeWeight > prev->nodeWeight)
					{
						prev->nodeWeight = newNodeWeight;
					}
					reverseQueue.push(prev);
				}
			}

			sort(renderPasses.begin(), renderPasses.end(), [](FrameGraphRenderPassNode* a, FrameGraphRenderPassNode* b) {
				return (a->nodeWeight > b->nodeWeight);
			});
			for (renderPassesIt = renderPasses.begin(); renderPassesIt != renderPasses.end() && (*renderPassesIt)->nodeWeight > 0; ++renderPassesIt);

			sort(resources.begin(), resources.end(), [](FrameGraphResourceNode* a, FrameGraphResourceNode* b) {
				return (a->nodeWeight > b->nodeWeight);
			});
			for (resourcesIt = resources.begin(); resourcesIt != resources.end() && (*resourcesIt)->nodeWeight > 0; ++resourcesIt);
		}

		void FrameGraph::AllocateGpuResources()
		{
			for (auto resourceIt = resources.begin(); resourceIt != resourcesIt; ++resourceIt)
			{
				auto& resource = *resourceIt;
				if (resource->resourceAlias == resource->resourceId)
				{
					gpuResources.push_back(rendererSystem.CreateTextureResource(resource->description));
				}
			}
		}

		void FrameGraph::Render()
		{
			for (auto renderIt = renderPasses.begin(); renderIt != renderPassesIt; ++renderIt)
			{
				(*renderIt)->renderCallback();
			}
		}

		void FrameGraph::FreeGpuResources()
		{
			for (auto gpuResource : gpuResources)
			{
				rendererSystem.FreeTextureResource(*gpuResource);
			}
		}
	}
}
