#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class ChromaticAberrationNode : public PostProcessNode
{
public:
	ChromaticAberrationNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, float amount = 0);
	void ProcessImpl() override;

	static constexpr auto IN_AMOUNT = "Amount";


protected:
	void SetUniforms() override;

private:
	size_t _amountParameterIndex;
	float _amount;
};

}
}
