#include "Node/Geometry/GeometrySourceNode.h"

namespace cascade {
namespace node {

namespace gl = ci::gl;

GeometrySourceNode::GeometrySourceNode()
{
	ci::gl::GlslProgRef shader;
	shader = gl::context()->getStockShader(gl::ShaderDef().color());
	_geometryData.Geometry = gl::Batch::create(ci::geom::Cube(), shader);

	_geometryParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Output, OUT_GEOMETRY, std::shared_ptr<data::GeometryDataObject>(&_geometryData));
}

void GeometrySourceNode::ProcessImpl()
{
	_geometryData.Transform = ci::mat4();
	_geometryData.Color = ci::Color::hex(0x00A8E8);
}

}
}