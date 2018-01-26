#include "Pch.h"
#include "FrameGraph.h"

using namespace std;

namespace Viper
{
	namespace Graphics
	{
		RTTI_DEFINITIONS(FrameGraphRenderPassNode)

		FrameGraphRenderPassNode::FrameGraphRenderPassNode(const string& name)
			: name(name)
		{
		}


		RTTI_DEFINITIONS(FrameGraphResourceNode)

		FrameGraphResourceNode::FrameGraphResourceNode(const TextureDescription& description)
			: resourceId(0), resourceAlias(0), description(description), isResourceReady(false), gpuResource(nullptr)
		{
		}


		FrameGraph::FrameGraph(ServiceLocator& serviceLocator)
			: graphRoot(nullptr), graphEnd(nullptr), rendererSystem(serviceLocator.GetRendererSystem())
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
			CullUnusedNodes();
			CalculateRenderPassSequence();
		}

		void FrameGraph::AllocateGpuResources()
		{
			for (auto resource : resources)
			{
				if (resource->resourceAlias == resource->resourceId)
				{
					gpuResources.push_back(rendererSystem.CreateTextureResource(resource->description));
				}
			}
		}

		void FrameGraph::Render()
		{
			for (auto renderPass : renderPasses)
			{
				renderPass->renderCallback();
			}
		}

		void FrameGraph::FreeGpuResources()
		{
			for (auto gpuResource : gpuResources)
			{
				rendererSystem.FreeTextureResource(*gpuResource);
			}
		}

		void FrameGraph::CullUnusedNodes()
		{
		}

		void FrameGraph::CalculateRenderPassSequence()
		{
			renderPasses.clear();
			vector<FrameGraphNode*> execQueue;
			execQueue.push_back(graphRoot);

			while (!execQueue.empty())
			{
				FrameGraphNode* node = execQueue.front();
				execQueue.pop_back();

				FrameGraphRenderPassNode* renderPass = node->As<FrameGraphRenderPassNode>();
				if (renderPass != nullptr)
				{
					bool processThisNode = true;
					for (auto input : renderPass->previous)
					{
						auto resource = input->As<FrameGraphResourceNode>();
						if (!resource->isResourceReady)
						{
							processThisNode = false;
							break;
						}
					}

					if (processThisNode)
					{
						renderPasses.push_back(renderPass);
						for (auto output : renderPass->next)
						{
							auto resource = output->As<FrameGraphResourceNode>();
							if (resource != nullptr)
							{
								resource->isResourceReady = true;
							}
						}
					}
				}

				for (auto output : node->next)
				{
					execQueue.push_back(output);
				}
			}
			assert(!renderPasses.empty());
		}
	}
}
