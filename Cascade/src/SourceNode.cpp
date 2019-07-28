#include "SourceNode.h"

namespace cascade {
namespace node {

SourceNode::SourceNode(const audio::MonitorSpectralNodeRef& audioSpectralMonitor) :
	_monitorSpectralNode(audioSpectralMonitor)
{
	AddParameter<float>(ParameterDirection::Output, "Volume");

	_outputCollection[0]->SetValue(100.0f);
}

SourceNode::~SourceNode()
{
}

void SourceNode::Process()
{
	float vol = _monitorSpectralNode->getVolume();
	_outputCollection[0]->SetValue(vol);
}

}
}