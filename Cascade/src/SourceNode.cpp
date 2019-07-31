#include "SourceNode.h"

namespace cascade {
namespace node {

SourceNode::SourceNode(const audio::MonitorSpectralNodeRef& audioSpectralMonitor) :
	_monitorSpectralNode(audioSpectralMonitor)
{
	_volumeParameterIndex = AddParameter<float>(ParameterDirection::Output, "Volume");
	_spectrumParameterIndex = AddParameter<std::vector<float>>(ParameterDirection::Output, "Spectrum");
}

SourceNode::~SourceNode()
{
}

void SourceNode::ProcessImpl()
{
	float vol = _monitorSpectralNode->getVolume();
	_outputs[_volumeParameterIndex]->SetValue(vol);

	auto spectrum = _monitorSpectralNode->getMagSpectrum();
	_outputs[_spectrumParameterIndex]->SetValue(spectrum);
}

}
}