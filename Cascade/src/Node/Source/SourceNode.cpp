#include "Node/Source/SourceNode.h"

namespace cascade {
namespace node {

SourceNode::SourceNode(const ci::audio::MonitorSpectralNodeRef& audioSpectralMonitor) :
	_monitorSpectralNode(audioSpectralMonitor)
{
	_volumeParameterIndex = AddParameter<float>(ParameterDirection::Output, OUT_VOLUME);
	_spectrumParameterIndex = AddParameter<std::vector<float>>(ParameterDirection::Output, OUT_SPECTRUM);
}

void SourceNode::ProcessImpl()
{
	float vol = _monitorSpectralNode->getVolume();
	_outputs[_volumeParameterIndex].SetValue(vol);

	auto spectrum = _monitorSpectralNode->getMagSpectrum();
	_outputs[_spectrumParameterIndex].SetValue(spectrum);
}

}
}