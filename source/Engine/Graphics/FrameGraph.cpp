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
			: graphRoot(nullptr), graphEnd(new FrameGraphNode("Present")), builder(*this), rendererSystem(serviceLocator.GetRendererSystem())
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
			delete graphEnd;
		}

		void FrameGraph::SetDisplayTarget(FrameGraphResourceNode& resource)
		{
			resource.next.push_back(graphEnd);
			graphEnd->previous.push_back(&resource);
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
				if (resource->nodeWeight == 0 && resource != graphEnd)
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
			for (auto resourceIt = resources.begin(); resourceIt != resources.end(); ++resourceIt)
			{
				auto& resource = *resourceIt;
				if (resource->resourceAlias == resource->resourceId)
				{
					resource->gpuResource = rendererSystem.CreateTextureResource(resource->description);
					assert(resource->gpuResource != nullptr);
					gpuResources.push_back(resource->gpuResource);
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
			unordered_map<int, vector<FrameGraphNode*>> nodeMap;
			vector<uint32_t> nodeWeights;

			activeRenderPasses << "\tnode [style=filled, fillcolor=\"orange\", fontcolor=\"white\", shape=rect];";
			inactiveRenderPasses << "\tnode [fontcolor=\"black\", shape=rect];";
			uint32_t renderPassIndex;
			for (renderPassIndex = 0; renderPassIndex < renderPasses.size(); ++renderPassIndex)
			{
				auto& renderPass = renderPasses[renderPassIndex];

				// node shape declaration
				auto& nodeCategory = (renderPass->nodeWeight > 0) ? activeRenderPasses : inactiveRenderPasses;
				nodeCategory << " " << renderPass->name << ";";

				// adding to rank map
				if (nodeMap.find(renderPass->nodeWeight) == nodeMap.end())
				{
					vector<FrameGraphNode*> nodes;
					nodeMap[renderPass->nodeWeight] = nodes;
					nodeWeights.push_back(renderPass->nodeWeight);
				}
				nodeMap[renderPass->nodeWeight].push_back(renderPass);

				// edges declaration
				auto& renderPassNext = renderPass->next;
				if (renderPassNext.empty() || (renderPassNext.size() == 1 && renderPassNext[0] == graphEnd))
				{
					continue;
				}
				edges << "\t" << renderPass->name << " -> { ";
				bool isInactive = false;
				for (uint32_t i = 0 ; i < renderPass->next.size(); ++i)
				{
					auto& output = renderPass->next[i];
					isInactive = output->nodeWeight == 0;
					if (isInactive && i > 0)
					{
						edges << "}" << endl << "\t" << renderPass->name << " -> { ";
					}
					edges << output->name << " ";
					if (isInactive)
					{
						edges << "} [style=dotted];" << endl;
						if (i < renderPass->next.size() - 1)
						{
							edges << "\t" << renderPass->name << " -> { ";
						}
					}
				}
				if (!isInactive)
				{
					edges << "}" << endl;
				}
			}

			activeResources << "\tnode [style=filled, fillcolor=\"skyblue3\", fontcolor=\"white\", shape=ellipse];";
			inactiveResources << "\tnode [fontcolor=\"black\", shape=ellipse];";
			uint32_t resourceIndex;
			for (resourceIndex = 0; resourceIndex < resources.size(); ++resourceIndex)
			{
				auto& resource = resources[resourceIndex];

				// node shape declaration
				auto& nodeCategory = (resource->nodeWeight > 0) ? activeResources : inactiveResources;
				nodeCategory << " " << resource->name << ";";

				// adding to rank map
				if (nodeMap.find(resource->nodeWeight) == nodeMap.end())
				{
					vector<FrameGraphNode*> nodes;
					nodeMap[resource->nodeWeight] = nodes;
					nodeWeights.push_back(resource->nodeWeight);
				}
				nodeMap[resource->nodeWeight].push_back(resource);

				// edges declaration
				auto& resourceNext = resource->next;
				if (resourceNext.empty() || (resourceNext.size() == 1 && resourceNext[0] == graphEnd))
				{
					continue;
				}
				edges << "\t" << resource->name << " -> { ";
				bool isInactive = false;
				for (uint32_t i = 0; i < resource->next.size(); ++i)
				{
					auto& output = resource->next[i];
					isInactive = output->nodeWeight == 0;
					if (isInactive && i > 0)
					{
						edges << "}" << endl << "\t" << resource->name << " -> { ";
					}
					edges << output->name << " ";
					if (isInactive)
					{
						edges << "} [style=dotted];" << endl;
						if (i < resource->next.size() - 1)
						{
							edges << "\t" << resource->name << " -> { ";
						}
					}
				}
				if (!isInactive)
				{
					edges << "}" << endl;
				}
			}

			sort(nodeWeights.begin(), nodeWeights.end(), [](uint32_t a, uint32_t b) {
				return (a > b);
			});

			for (auto weight : nodeWeights)
			{
				ranks << "\t{ rank=same; ";
				auto& nodes = nodeMap[weight];
				for (uint32_t i = 0; i < nodes.size(); ++i)
				{
					if (i > 0)
					{
						ranks << ", ";
					}
					ranks << nodes[i]->name;
				}
				ranks << " }" << endl;
			}

			file << "digraph FrameGraph {" << endl << "\trankdir=LR;" << endl;
			file << inactiveRenderPasses.str() << endl;
			file << inactiveResources.str() << endl;
			file << activeRenderPasses.str() << endl;
			file << activeResources.str() << endl << endl;
			file << edges.str() << endl;
			file << ranks.str() << endl;
			file << "\toverlap = false;" << endl;
			file << "\tlabel = \"" << graphName << "\";" << endl;
			file << "\tlabelloc = \"t\";" << endl;
			file << "}";
			file.close();
		}
	}
}
