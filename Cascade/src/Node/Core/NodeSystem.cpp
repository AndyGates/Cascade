#include "Node/Core/NodeSystem.h"
#include "Node/Core/Node.h"
#include "Node/Core/IRenderNode.h"

namespace cascade {
namespace node {

NodeSystem::NodeSystem()
{
}

NodeSystem::~NodeSystem()
{
}

size_t NodeSystem::AddNode(const std::shared_ptr<Node>& node)
{
	size_t index = -1;
	
	if (node != nullptr)
	{
		_nodes.push_back(node);
		index = _nodes.size();

		//If this node requires rendering, push it into the render vector
		//This could be decoupled, so the nodes do not do the rendering but produce the information for another system to do so?
		const std::shared_ptr<IRenderNode>& renderNode = std::dynamic_pointer_cast<IRenderNode>(node);
		if (renderNode != nullptr)
		{
			_renderNodes.push_back(renderNode);
		}
	}

	return index;
}

void NodeSystem::Process()
{
	for (auto n : _nodes)
	{
		n->Process();
	}

	//Mark the nodes as complete, reset for next cycle
	for (auto n : _nodes)
	{
		n->ProcessComplete();
	}
}

void NodeSystem::Render()
{
	for (auto n : _renderNodes)
	{
		n->Render();
	}
}

}
}