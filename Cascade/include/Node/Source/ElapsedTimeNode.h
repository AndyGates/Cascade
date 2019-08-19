#pragma once

#include "Node/Core/Node.h"

namespace cascade {
namespace node {

class ElapsedTimeNode : public Node
{
public:
	ElapsedTimeNode();
	~ElapsedTimeNode();

	virtual void ProcessImpl() override;

private:
	size_t _timeParameterIndex;
};

}
}
