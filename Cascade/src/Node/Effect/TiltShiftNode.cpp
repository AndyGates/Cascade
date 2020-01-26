#include "Node/Effect/TiltShiftNode.h"
#include "cinder/app/App.h"
#include <exception>

namespace cascade {
namespace node {

TiltShiftNode::TiltShiftNode(gl::FboRef primaryRenderTarget, gl::FboRef secondaryRenderTarget, float blurRadius, float distance) : 
_primaryRenderTarget(primaryRenderTarget), 
_secondaryRenderTarget(secondaryRenderTarget),
_distanceParameterIndex(),
_distance(distance),
_blurRadiusParameterIndex(),
_blurRadius(blurRadius)
{
	try
	{
		_prog = gl::GlslProg::create(cinder::app::loadAsset("Passthrough.vert"), cinder::app::loadAsset("TiltShift.frag"));
	}
	catch (cinder::Exception &ex)
	{
		throw std::invalid_argument("Failed to create pixel shader");
	}

	_distanceParameterIndex = AddParameter<float>(ParameterDirection::Input, "Distance", _distance);
}

void TiltShiftNode::ProcessImpl()
{
	if (_inputConnected[_distanceParameterIndex])
	{
		auto ptr = _inputs[_distanceParameterIndex]->GetValue<float>();
		_distance = *ptr;
	}
}

void TiltShiftNode::Render()
{
	gl::ScopedGlslProg prog(_prog);
	RenderPass(_primaryRenderTarget, _secondaryRenderTarget, cinder::vec2(1, 0));
	RenderPass(_secondaryRenderTarget, _primaryRenderTarget, cinder::vec2(0, 1));
	RenderPass(_primaryRenderTarget, _secondaryRenderTarget, cinder::vec2(1, 0));
	RenderPass(_secondaryRenderTarget, _primaryRenderTarget, cinder::vec2(0, 1));
}

void TiltShiftNode::RenderPass(gl::FboRef renderTarget, gl::FboRef texture, const cinder::vec2 direction)
{
	gl::ScopedFramebuffer fb(renderTarget);
	gl::ScopedTextureBind tex0(texture->getColorTexture(), static_cast<uint8_t>(0));

	_prog->uniform("_distance", _distance);
	_prog->uniform("_blurRadius", _blurRadius);
	_prog->uniform("_direction", direction);

	gl::drawSolidRect(renderTarget->getBounds());

}

}
}
