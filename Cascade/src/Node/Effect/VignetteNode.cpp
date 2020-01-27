#include "Node/Effect/VignetteNode.h"

namespace cascade {
namespace node {

VignetteNode::VignetteNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const ci::vec2& resolution, float amount) : PostProcessNode(renderTarget, textureBuffer, "Vignette.frag"), 
_amountParameterIndex(),
_resolution(resolution),
_amount(amount)
{
	_amountParameterIndex = AddParameter<float>(ParameterDirection::Input, "Amount", _amount);
}

void VignetteNode::ProcessImpl()
{
	if (_inputConnected[_amountParameterIndex])
	{
		auto ptr = _inputs[_amountParameterIndex].GetValue<float>();
		_amount = *ptr;
	}
}

void VignetteNode::SetUniforms()
{
	SetUniform("_amount", _amount);
	SetUniform("_resolution", _resolution);
}

}
}
