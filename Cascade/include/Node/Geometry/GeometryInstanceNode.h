#pragma once
#include "cinder/gl/gl.h"

#include "Node/Core/Node.h"
#include "Data/GeometryDataObject.h"

namespace cascade {
namespace node {

namespace gl = ci::gl;

class GeometryInstanceNode : public Node
{
public:
	GeometryInstanceNode();
	void ProcessImpl() override;

private:
	std::shared_ptr<data::GeometryDataObject> _geometryData;

	size_t									  _instanceCountParameterIndex;
	size_t									  _geometryInParameterIndex;
	size_t									  _geometryOutParameterIndex;
};

}
}
