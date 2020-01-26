#pragma once
#include "Node/Core/IRenderNode.h"
#include "Node/Core/Node.h"

#include "cinder/gl/gl.h"

namespace cascade {
namespace node {

namespace gl = ci::gl;

class PostProcessNode : public Node, public IRenderNode
{
public:
	PostProcessNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const std::string& pixelShader);
	virtual ~PostProcessNode() override = default;
	 
	void Render() override;

protected: 
	virtual void SetUniforms() = 0;

	template<class T>
	void SetUniform(const std::string& name, T value);

private:
	gl::GlslProgRef	_prog;
	gl::FboRef		_renderTarget;
	gl::FboRef		_textureBuffer;
};

template<class T>
void PostProcessNode::SetUniform(const std::string& name, T value)
{
	_prog->uniform(name, value);
}

}
}

