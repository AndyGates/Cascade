#include "Node/Effect/PostProcessNode.h"
#include "cinder/Filesystem.h"
#include "cinder/app/App.h"

#include <exception>

namespace cascade {
namespace node {

PostProcessNode::PostProcessNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const std::string& pixelShaderAsset) :
	_renderTarget(renderTarget), _textureBuffer(textureBuffer)
{
	try
	{
		_prog = gl::GlslProg::create(cinder::app::loadAsset("Passthrough.vert"), cinder::app::loadAsset(pixelShaderAsset));
	}
	catch (ci::Exception &ex)
	{
		throw std::invalid_argument("Failed to create pixel shader");
	}
}

void PostProcessNode::Render()
{
	if (_renderTarget != nullptr)
	{
		_renderTarget->bindFramebuffer();
	}

	gl::ScopedTextureBind tex0(_textureBuffer->getColorTexture(), static_cast<uint8_t>(0));
	gl::ScopedGlslProg prog(_prog);

	SetUniforms();

	gl::drawSolidRect(_textureBuffer->getBounds());
	
	if (_renderTarget != nullptr)
	{
		_renderTarget->unbindFramebuffer();
	}
}

}
}
