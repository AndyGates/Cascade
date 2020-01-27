#include "Node/Core/NodeSystem.h"
#include "Node/Core/Node.h"
#include "Node/Core/IRenderNode.h"

namespace cascade {
namespace node {

NodeSystem::NodeSystem()
{
}

size_t NodeSystem::AddNode(std::unique_ptr<Node> node)
{
	size_t index = -1;
	
	if (node != nullptr)
	{
		//Determine if node is a render node by attempting a dynamic_cast
		IRenderNode* renderNode = dynamic_cast<IRenderNode*>(node.get());
		if (renderNode != nullptr)
		{
			//If this node requires rendering, push it into the render vector
			//This could be decoupled, so the nodes do not do the rendering but produce the information for another system to do so?
			_renderNodes.push_back(renderNode);
		}

		//Give the vector ownership of the unique_ptr
		_nodes.push_back(std::move(node));
		index = _nodes.size();
	}

	return index;
}

void NodeSystem::Process()
{
	//A bit ham fisted at the moment
	//Nodes process recursively backwards so we only need to trigger the end nodes. 
	for (auto& node : _nodes)
	{
		if (node->IsOutputNode())
		{
			node->Process();
		}
	}

	//Mark the nodes as complete, reset for next cycle
	for (auto& node : _nodes)
	{
		node->ProcessComplete();
	}
}

void NodeSystem::Render()
{
	for (auto node : _renderNodes)
	{
		node->Render();
	}
}

}
}