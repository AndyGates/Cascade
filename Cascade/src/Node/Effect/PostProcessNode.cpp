#include "Node/Effect/PostProcessNode.h"
#include "cinder/Filesystem.h"
#include "cinder/app/App.h"

namespace cascade {
namespace node {

PostProcessNode::PostProcessNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const std::string& pixelShaderAsset) :
	_renderTarget(renderTarget), _textureBuffer(textureBuffer)
{
	_prog = gl::GlslProg::create(cinder::app::loadAsset("Passthrough.vert"), cinder::app::loadAsset(pixelShaderAsset));
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
