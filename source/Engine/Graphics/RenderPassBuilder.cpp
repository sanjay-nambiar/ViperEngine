#include "Pch.h"
#include "RenderPassBuilder.h"
#include "FrameGraph.h"

using namespace std;

namespace Viper
{
	namespace Graphics
	{
		RenderPassBuilder::RenderPassBuilder(FrameGraph& frameGraph)
			: frameGraph(frameGraph), currentRenderPass(nullptr), resourceId(0)
		{
		}

		void RenderPassBuilder::Read(FrameGraphResourceNode& input)
		{
			currentRenderPass->previous.push_back(&input);
			input.next.push_back(currentRenderPass);
		}

		void RenderPassBuilder::Write(FrameGraphResourceNode& output)
		{
			FrameGraphResourceNode* resource = new FrameGraphResourceNode(output.description);
			resource->resourceId = ++resourceId;
			if (output.resourceAlias == 0)
			{
				resource->resourceAlias = resource->resourceId;
			}
			currentRenderPass->next.push_back(resource);
			resource->previous.push_back(currentRenderPass);
			frameGraph.resources.push_back(resource);
		}

		void RenderPassBuilder::CreateWrite(const TextureDescription& description)
		{
			FrameGraphResourceNode resource(description);
			Write(resource);
		}

		FrameGraphRenderPassNode& RenderPassBuilder::AddRenderPassNode(const string& name)
		{
			currentRenderPass = new FrameGraphRenderPassNode(name);
			return *currentRenderPass;
		}
	}
}
