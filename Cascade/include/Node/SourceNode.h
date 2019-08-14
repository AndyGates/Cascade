#pragma once

#include "Core/Node.h"
#include "cinder/audio/audio.h"

using namespace ci;

namespace cascade {
namespace node {

class SourceNode : public Node
{
public:
	SourceNode(const audio::MonitorSpectralNodeRef& audioSpectralMonitor);
	~SourceNode();

	virtual void ProcessImpl() override;

private:
	size_t _volumeParameterIndex;
	size_t _spectrumParameterIndex;

	audio::MonitorSpectralNodeRef _monitorSpectralNode;
};

}
}
