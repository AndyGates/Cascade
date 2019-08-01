#include "ChromaticAberrationNode.h"

namespace cascade {
namespace node {

ChromaticAberrationNode::ChromaticAberrationNode(gl::FboRef renderTexture, gl::FboRef textureBuffer, float amount) : PostProcessNode(renderTexture, textureBuffer, "ChromaticAberration.frag"),
_amountParameterIndex(),
_amount(amount)
{
	_amountParameterIndex = AddParameter<float>(ParameterDirection::Input, "Amount", amount);
}

ChromaticAberrationNode::~ChromaticAberrationNode()
{
}

void ChromaticAberrationNode::ProcessImpl()
{
	auto ptr = _inputs[_amountParameterIndex]->GetValue<float>();
	const float in = *ptr;
	_amount = in / 40.0f;
}

void ChromaticAberrationNode::SetUniforms()
{
	SetUniform("amount", _amount);
}

}
}
