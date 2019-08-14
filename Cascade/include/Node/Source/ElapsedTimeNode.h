#pragma once

#include "Node/Core/Node.h"

using namespace ci;

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
