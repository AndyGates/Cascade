#pragma once

namespace cascade {
namespace node {

class Node;

struct Connection
{
public:
	Connection(Node* outputNode, size_t outputIndex, size_t inputIndex) 
		: OutputNode(outputNode), OutputIndex(outputIndex), InputIndex(inputIndex) 
	{
	}

	~Connection() = default;

	//Store pointer to output node so we can trigger the process
	Node* OutputNode;

	size_t OutputIndex;
	size_t InputIndex;
};

}
}

