#pragma once

#include <memory>

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

	std::shared_ptr<Node> OutputNode;
	size_t OutputIndex;
	size_t InputIndex;
};

}
}

