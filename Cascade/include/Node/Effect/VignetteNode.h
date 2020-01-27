#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class VignetteNode : public PostProcessNode
{
public:
	VignetteNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, const ci::vec2& resolution, float amount = 0);

	void ProcessImpl() override;

	static constexpr auto IN_AMOUNT = "Amount";

protected:
	void SetUniforms() override;

private:
	size_t _amountParameterIndex;
	float _amount;
	ci::vec2 _resolution;
};

}
}
