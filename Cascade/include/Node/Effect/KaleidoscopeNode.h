#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class KaleidoscopeNode : public PostProcessNode
{
public:
	KaleidoscopeNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, int amount = 3);
	void ProcessImpl() override;

	static constexpr auto IN_AMOUNT = "Amount";

protected:
	void SetUniforms() override;

private:
	size_t _amountParameterIndex;
	int _amount;
};

}
}
