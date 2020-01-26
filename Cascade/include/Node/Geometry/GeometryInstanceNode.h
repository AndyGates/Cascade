#pragma once

#include "Node/Core/Node.h"
#include "Data/GeometryDataObject.h"

namespace cascade {
namespace node {

class GeometryInstanceNode : public Node
{
public:
	GeometryInstanceNode();
	void ProcessImpl() override;

private:
	std::shared_ptr<data::GeometryDataObject> _geometryData;

	size_t _geometryInParameterIndex;
	size_t _tempInParameterIndex;

	size_t _geometryOutParameterIndex;
};

}
}
