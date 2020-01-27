#pragma once

#include "Node/Core/Node.h"

namespace cascade {
namespace node {

class ElapsedTimeNode : public Node
{
public:
	ElapsedTimeNode();
	virtual void ProcessImpl() override;

	static constexpr auto OUT_TIME = "Time";

private:
	size_t _timeParameterIndex;
};

}
}
