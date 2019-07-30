#include "ChromaticAberrationNode.h"

namespace cascade {
namespace node {

ChromaticAberrationNode::ChromaticAberrationNode(const gl::FboRef& renderTexture) : PostProcessNode(renderTexture, "ChromaticAberration.frag"), _amountParameterIndex()
{
	_amountParameterIndex = AddParameter<float>(ParameterDirection::Input, "Amount");
	_inputs[0].SetValue(100.0f);
}

ChromaticAberrationNode::~ChromaticAberrationNode()
{
}

void ChromaticAberrationNode::ProcessImpl()
{
	auto ptr = _inputs[_amountParameterIndex].GetValue<float>();
	const float& in = *ptr;
	_amount = in / 40.0f;
}

void ChromaticAberrationNode::SetUniforms()
{
	SetUniform("amount", _amount);
}

}
}
