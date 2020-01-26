#include "Node/Source/ElapsedTimeNode.h"

namespace cascade {
namespace node {

ElapsedTimeNode::ElapsedTimeNode()
{
	_timeParameterIndex = AddParameter<float>(ParameterDirection::Output, "Time");
}

void ElapsedTimeNode::ProcessImpl()
{

}

}
}