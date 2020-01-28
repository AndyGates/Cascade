#include "Node/Source/ElapsedTimeNode.h"
#include "cinder/app/App.h"

namespace cascade {
namespace node {

ElapsedTimeNode::ElapsedTimeNode()
{
	_timeParameterIndex = AddParameter<float>(ParameterDirection::Output, OUT_TIME);
}

void ElapsedTimeNode::ProcessImpl()
{
	float time = static_cast<float>(ci::app::getElapsedSeconds());
	_outputs[_timeParameterIndex].SetValue(time);
}

}
}