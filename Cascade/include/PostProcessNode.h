#pragma once
#include "IRenderNode.h"
#include "Node.h"

#include "cinder/gl/gl.h"

using namespace cinder;

namespace cascade {
namespace node {

class PostProcessNode : public Node, public IRenderNode
{
public:
	PostProcessNode(gl::FboRef renderTexture, gl::FboRef textureBuffer, const std::string& pixelShader);
	virtual ~PostProcessNode() override;
	 
	void Render() override;

protected: 
	virtual void SetUniforms() = 0;

	template<class T>
	void SetUniform(const std::string& name, T value);

private:
	gl::GlslProgRef	_prog;
	gl::FboRef		_renderTexture;
	gl::FboRef		_textureBuffer;
};

template<class T>
void PostProcessNode::SetUniform(const std::string& name, T value)
{
	_prog->uniform(name, value);
}

}
}

