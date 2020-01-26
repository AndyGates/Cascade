#pragma once

namespace cascade {
namespace node {

class Node;

struct Connection
{
public:
	Connection(const std::shared_ptr<Node>& outputNode, size_t outputIndex, size_t inputIndex) 
		: OutputNode(outputNode), OutputIndex(outputIndex), InputIndex(inputIndex) 
	{
	}

	//Store pointer to output node so we can trigger the process
	std::shared_ptr<Node> OutputNode;
	size_t OutputIndex;
	size_t InputIndex;
};

}
}

