#include "ChromaticAberrationNode.h"

namespace cascade {
namespace node {

ChromaticAberrationNode::ChromaticAberrationNode(const gl::FboRef& renderTexture) : PostProcessNode(renderTexture, "ChromaticAberration.frag")
{
	AddParameter<float>(ParameterDirection::Input, "Amount");
}

ChromaticAberrationNode::~ChromaticAberrationNode()
{
}

void ChromaticAberrationNode::Process()
{
	const float& in = *_inputCollection[0]->GetValue<float>();
	_amount = in / 40.0f;
}

void ChromaticAberrationNode::SetUniforms()
{
	SetUniform("amount", _amount);
}

}
}
