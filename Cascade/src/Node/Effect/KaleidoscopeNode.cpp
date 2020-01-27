#include "Node/Effect/KaleidoscopeNode.h"

namespace cascade {
namespace node {

KaleidoscopeNode::KaleidoscopeNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, int amount) : PostProcessNode(renderTarget, textureBuffer, "Kaleidoscope.frag"),
_amountParameterIndex(),
_amount(amount)
{
	_amountParameterIndex = AddParameter<int>(ParameterDirection::Input, "Amount", amount);
}

void KaleidoscopeNode::ProcessImpl()
{
	if (_inputConnected[_amountParameterIndex])
	{
		auto ptr = _inputs[_amountParameterIndex].GetValue<int>();
		_amount = *ptr;
	}
}

void KaleidoscopeNode::SetUniforms()
{
	SetUniform("_amount", _amount);
}

}
}
