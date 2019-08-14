#pragma once

#include <vector>
#include <memory>

namespace cascade {
namespace node {

class Node;
class IRenderNode;

class NodeSystem
{
public:
	NodeSystem();
	~NodeSystem();

	//Currently nodes are defined at compile time, so no remove
	//This may need to change if nodes are added/removed on the fly
	size_t AddNode(const std::shared_ptr<Node>& node);

	void Process();
	void Render();

private:
	std::vector<std::shared_ptr<Node>> _nodes;
	std::vector<std::shared_ptr<IRenderNode>> _renderNodes;
};

}
}

