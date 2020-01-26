#include "Node/Geometry/GeometrySourceNode.h"

namespace cascade {
namespace node {

namespace gl = ci::gl;

GeometrySourceNode::GeometrySourceNode()
{
	ci::gl::GlslProgRef shader;

	try
	{
		shader = gl::context()->getStockShader(gl::ShaderDef().color());
	}
	catch (ci::Exception &ex)
	{
		CI_LOG_E("Error loading shader: " << ex.what());
		throw;
	}

	try
	{
		_geometryData.Geometry = gl::Batch::create(ci::geom::Cube(), shader);
	}
	catch (ci::Exception &ex)
	{
		CI_LOG_E("Error creating geometry: " << ex.what());
		throw;
	}

	_geometryParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Output, "Geometry", std::shared_ptr<data::GeometryDataObject>(&_geometryData));
}

void GeometrySourceNode::ProcessImpl()
{
	_geometryData.Transform = ci::mat4();
	_geometryData.Color = ci::Color();
}

}
}