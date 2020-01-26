#include "Node/Source/ElapsedTimeNode.h"
#include "cinder/app/App.h"

namespace cascade {
namespace node {

ElapsedTimeNode::ElapsedTimeNode()
{
	_timeParameterIndex = AddParameter<float>(ParameterDirection::Output, "Time");
}

void ElapsedTimeNode::ProcessImpl()
{
	float time = ci::app::getElapsedSeconds();
	_outputs[_timeParameterIndex]->SetValue(time);
}

}
}