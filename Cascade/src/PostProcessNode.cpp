#include "PostProcessNode.h"
#include "cinder/Filesystem.h"
#include "cinder/app/App.h"

#include <exception>

using namespace cinder::app;

namespace cascade {
namespace node {

PostProcessNode::PostProcessNode(const gl::FboRef& renderTexture, const std::string& pixelShaderAsset) :
	_renderTexture(renderTexture)
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
	const gl::ScopedViewport scopedViewport(ivec2(0), _renderTexture->getSize());

	gl::ScopedGlslProg prog(_prog);

	gl::ScopedTextureBind tex0(_renderTexture->getColorTexture(), static_cast<uint8_t>(0));

	gl::drawSolidRect(_renderTexture->getBounds());
}

}
}
