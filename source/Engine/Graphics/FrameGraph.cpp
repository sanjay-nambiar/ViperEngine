#include "Pch.h"
#include "FrameGraph.h"

using namespace std;

namespace Viper
{
	namespace Graphics
	{
		FrameGraphNode::FrameGraphNode(const std::string& name)
			: name(name), nodeWeight(0)
		{
		}


		FrameGraphRenderPassNode::FrameGraphRenderPassNode(const string& name)
			: FrameGraphNode(name)
		{
		}



		FrameGraphResourceNode::FrameGraphResourceNode(const std::string& name, const TextureDescription& description)
			: FrameGraphNode(name), resourceId(0), resourceAlias(0), description(description), isResourceReady(false), gpuResource(nullptr)
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
			// Calculate dependency based node weights. Zero weight nodes have no effect on output.
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

			// Sort render passes based on node weights and find the point from were no effect nodes start in the vector
			sort(renderPasses.begin(), renderPasses.end(), [](FrameGraphRenderPassNode* a, FrameGraphRenderPassNode* b) {
				return (a->nodeWeight > b->nodeWeight);
			});
			for (renderPassesIt = renderPasses.begin(); renderPassesIt != renderPasses.end() && (*renderPassesIt)->nodeWeight > 0; ++renderPassesIt);


			// Some zero node weight resources maybe necessary to be allocated since a valid renderpass might still be writing to it.
			// Bump up node weights for resources which doesn't affect the final output but is required for some passes to work.
			for (auto& resource : resources)
			{
				if (resource->nodeWeight == 0)
				{
					for (auto& prev : resource->previous)
					{
						if (prev->nodeWeight > 0)
						{
							resource->nodeWeight = 1;
						}
					}
				}
			}
			// Sort resouces based on node weights and find the point from were no effect nodes start in the vector
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

		void FrameGraph::DebugOutput(const string& graphName, const string& filename)
		{
			assert(graphRoot != nullptr);
			fstream file(filename, ios::out);
			stringstream activeRenderPasses;
			stringstream activeResources;
			stringstream inactiveRenderPasses;
			stringstream inactiveResources;
			stringstream edges;
			stringstream ranks;

			activeRenderPasses << "\tnode [style=filled, fillcolor=\"orange\", fontcolor=\"white\", shape=rect]; ";
			uint32_t renderPassIndex;
			for (renderPassIndex = 0; renderPassIndex < renderPasses.size(); ++renderPassIndex)
			{
				auto& renderPass = renderPasses[renderPassIndex];

				// edges declaration
				edges << "\t" << renderPass->name << " -> { ";
				for (auto& output : renderPass->next)
				{
					edges << output->name << " ";
				}
				edges << "}" << endl;

				// node shape declaration
				auto& nodeCategory = (renderPass->nodeWeight > 0) ? activeRenderPasses : inactiveRenderPasses;
				nodeCategory << renderPass->name << "; ";
			}

			activeResources << "\tnode [style=filled, fillcolor=\"skyblue3\", fontcolor=\"white\", shape=ellipse]; ";
			uint32_t resourceIndex;
			for (resourceIndex = 0; resourceIndex < resources.size() && resources[resourceIndex]->nodeWeight > 0; ++resourceIndex)
			{
				auto& resource = resources[resourceIndex];

				// edges declaration
				edges << "\t" << resource->name << " -> { ";
				for (auto& output : resource->next)
				{
					edges << output->name << " ";
				}
				edges << "}" << endl;

				// node shape declaration
				auto& nodeCategory = (resource->nodeWeight > 0) ? activeResources : inactiveResources;
				nodeCategory << resource->name << "; ";
			}

			vector<FrameGraphNode*> nodes;
			nodes.insert(end(nodes), begin(renderPasses), end(renderPasses));
			nodes.insert(end(nodes), begin(resources), end(resources));
			sort(nodes.begin(), nodes.end(), [](FrameGraphNode* a, FrameGraphNode* b) {
				return (a->nodeWeight > b->nodeWeight);
			});

			ranks << "\t{ rank = same; " << nodes[0]->name;
			uint32_t nodeWeight = nodes[0]->nodeWeight;
			for (uint32_t i = 1; i < nodes.size(); ++i)
			{
				auto& node = nodes[i];
				if (nodeWeight != node->nodeWeight)
				{
					nodeWeight = node->nodeWeight;
					ranks << " }";
					if (i < nodes.size() - 1)
					{
						ranks << endl << "\t{ rank = same; ";
					}
				}
				else
				{
					ranks << ", " << node->name;
				}
			}

			file << "digraph FrameGraph {" << endl << "\trankdir = LR;" << endl;
			file << activeRenderPasses.str() << endl;
			file << activeResources.str() << endl;
			file << inactiveRenderPasses.str() << endl;
			file << inactiveResources.str() << endl << endl;
			file << edges.str() << endl << endl;
			file << ranks.str() << endl << endl;
			file << "\toverlap = false;" << endl;
			file << "\tlabel = \"" << graphName << "\";" << endl;
			file << "\tlabelloc = \"t\";" << endl;
			file << "}" << endl;

			file.close();
		}
	}
}
