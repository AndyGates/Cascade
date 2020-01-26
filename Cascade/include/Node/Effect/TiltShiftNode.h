#pragma once

#include "Node/Core/Node.h"
#include "Node/Core/IRenderNode.h"
#include "cinder/gl/gl.h"

namespace cascade {
namespace node {

namespace gl = cinder::gl;

class TiltShiftNode : public Node, public IRenderNode
{
public:
	TiltShiftNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, float blurRadius = 0, float distance = 0);

	void ProcessImpl() override;
	void Render() override;

private:
	size_t _distanceParameterIndex;
	float _distance;

	size_t _blurRadiusParameterIndex;
	float _blurRadius;

	cinder::gl::GlslProgRef	_prog;
	cinder::gl::FboRef		_primaryRenderTarget;
	cinder::gl::FboRef		_secondaryRenderTarget;

	void RenderPass(gl::FboRef renderTarget, gl::FboRef texture, const cinder::vec2 direction);
};

}
}
