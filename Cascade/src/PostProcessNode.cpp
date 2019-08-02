#include "PostProcessNode.h"
#include "cinder/Filesystem.h"
#include "cinder/app/App.h"

#include <exception>

using namespace cinder::app;

namespace cascade {
namespace node {

PostProcessNode::PostProcessNode(gl::FboRef renderTexture, gl::FboRef textureBuffer, const std::string& pixelShaderAsset) :
	_renderTexture(renderTexture), _textureBuffer(textureBuffer)
{
	try
	{
		_prog = gl::GlslProg::create(loadAsset("Passthrough.vert"), loadAsset(pixelShaderAsset));
	}
	catch (Exception &ex)
	{
		throw std::invalid_argument("Failed to create pixel shader");
	}
}

PostProcessNode::~PostProcessNode()
{
}

void PostProcessNode::Render()
{
	if (_renderTexture != nullptr)
	{
		_renderTexture->bindFramebuffer();
	}

	gl::ScopedTextureBind tex0(_textureBuffer->getColorTexture(), static_cast<uint8_t>(0));
	gl::ScopedGlslProg prog(_prog);

	SetUniforms();

	gl::drawSolidRect(_textureBuffer->getBounds());
	
	if (_renderTexture != nullptr)
	{
		_renderTexture->unbindFramebuffer();
	}
}

}
}
