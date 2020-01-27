#pragma once
#include "cinder/gl/gl.h"

#include "Node/Core/Node.h"
#include "Node/Core/IRenderNode.h"
#include "Data/GeometryDataObject.h"

namespace cascade {
namespace node {

namespace gl = ci::gl;

class RenderNode : public Node, public IRenderNode
{
public:
	RenderNode(const ci::CameraOrtho& camera, ci::gl::FboRef renderTarget);
	~RenderNode();

	void ProcessImpl() override;
	void Render() override;
	void RenderInstance(glm::mat4 transform = {});

	static constexpr auto IN_GEOMETRY = "Geometry";

private:
	ci::gl::FboRef							  _renderTarget;
	const ci::CameraOrtho&					  _camera;

	std::shared_ptr<data::GeometryDataObject> _geometryData;
	size_t									  _geometryParameterIndex;
};

}
}
