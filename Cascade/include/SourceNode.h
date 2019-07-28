#pragma once

#include "Node.h"
#include "cinder/audio/audio.h"

using namespace ci;

namespace cascade {
namespace node {

class SourceNode : public Node
{
public:
	SourceNode(const audio::MonitorSpectralNodeRef& audioSpectralMonitor);
	~SourceNode();

	virtual void Process() override;

private:
	audio::MonitorSpectralNodeRef _monitorSpectralNode;
};

}
}
