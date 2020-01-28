#pragma once

#include "Node/Core/Node.h"
#include "Data/GeometryDataObject.h"

namespace cascade {
namespace node {

class GeometryInstanceNode : public Node
{
public:
	GeometryInstanceNode(uint32_t instanceCount, float radius);
	void ProcessImpl() override;
	
	static constexpr auto IN_TEMP		= "TempIn";
	static constexpr auto IN_GEOMETRY	= "GeometryIn";

	static constexpr auto OUT_GEOMETRY	= "GeometryOut";

private:
	std::shared_ptr<data::GeometryDataObject> _geometryData;

	size_t _geometryInParameterIndex;
	size_t _tempInParameterIndex;

	size_t _geometryOutParameterIndex;

	uint32_t _instanceCount;
	float _radius;
	float _angleCumulative;
};

}
}
