#include "Node/Effect/ChromaticAberrationNode.h"

namespace cascade {
namespace node {

ChromaticAberrationNode::ChromaticAberrationNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, float amount) : PostProcessNode(renderTarget, textureBuffer, "ChromaticAberration.frag"),
_amountParameterIndex(),
_amount(amount)
{
	_amountParameterIndex = AddParameter<float>(ParameterDirection::Input, "Amount", amount);
}

void ChromaticAberrationNode::ProcessImpl()
{
	auto ptr = _inputs[_amountParameterIndex].GetValue<float>();
	_amount = *ptr;
}

void ChromaticAberrationNode::SetUniforms()
{
	SetUniform("amount", _amount);
}

}
}
