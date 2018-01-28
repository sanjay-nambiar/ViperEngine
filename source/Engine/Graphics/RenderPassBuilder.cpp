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

		FrameGraphResourceNode& RenderPassBuilder::Read(FrameGraphResourceNode& input)
		{
			currentRenderPass->previous.push_back(&input);
			input.next.push_back(currentRenderPass);
			return input;
		}

		FrameGraphResourceNode& RenderPassBuilder::Write(const string& newName, FrameGraphResourceNode& output)
		{
			FrameGraphResourceNode* resource = new FrameGraphResourceNode(newName, output.description);
			resource->resourceId = ++resourceId;
			if (output.resourceAlias == 0)
			{
				resource->resourceAlias = resource->resourceId;
			}
			currentRenderPass->next.push_back(resource);
			resource->previous.push_back(currentRenderPass);
			frameGraph.resources.push_back(resource);
			return *resource;
		}

		FrameGraphResourceNode& RenderPassBuilder::CreateWrite(const string& name, const TextureDescription& description)
		{
			FrameGraphResourceNode resource(name, description);
			return Write(name, resource);
		}

		FrameGraphRenderPassNode& RenderPassBuilder::AddRenderPassNode(const string& name)
		{
			currentRenderPass = new FrameGraphRenderPassNode(name);
			return *currentRenderPass;
		}
	}
}
