#pragma once

#include "Node/Core/Node.h"
#include "cinder/audio/audio.h"

namespace cascade {
namespace node {

class SourceNode : public Node
{
public:
	SourceNode(const ci::audio::MonitorSpectralNodeRef& audioSpectralMonitor);
	virtual void ProcessImpl() override;

	static constexpr auto OUT_VOLUME	= "Volume";
	static constexpr auto OUT_SPECTRUM	= "Spectrum";

private:
	size_t _volumeParameterIndex;
	size_t _spectrumParameterIndex;

	ci::audio::MonitorSpectralNodeRef _monitorSpectralNode;
};

}
}
