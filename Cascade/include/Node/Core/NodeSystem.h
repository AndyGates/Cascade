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

	//Currently nodes are defined at compile time, so no remove
	//This may need to change if nodes are added/removed on the fly
	//We take ownership of the node at this point
	size_t AddNode(std::unique_ptr<Node> node);

	void Process();
	void Render();

private:
	std::vector<std::unique_ptr<Node>> _nodes;
	std::vector<IRenderNode*> _renderNodes;
};

}
}

