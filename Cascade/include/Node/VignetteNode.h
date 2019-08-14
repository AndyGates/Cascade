#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class VignetteNode : public PostProcessNode
{
public:
	VignetteNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const vec2& resolution, float amount = 0);
	~VignetteNode() override;

	void ProcessImpl() override;

protected:
	void SetUniforms() override;

private:
	size_t _amountParameterIndex;
	float _amount;
	vec2 _resolution;
};

}
}
