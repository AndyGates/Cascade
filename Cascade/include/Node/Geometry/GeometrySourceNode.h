#pragma once

#include "Node/Core/Node.h"
#include "Data/GeometryDataObject.h"

namespace cascade {
namespace node {

class GeometrySourceNode : public Node
{
public:
	GeometrySourceNode();
	virtual void ProcessImpl() override;

private:
	data::GeometryDataObject _geometryData;
	size_t					 _geometryParameterIndex;
};

}
}
