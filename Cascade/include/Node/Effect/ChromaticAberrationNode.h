#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class ChromaticAberrationNode : public PostProcessNode
{
public:
	ChromaticAberrationNode(gl::FboRef renderTarget, gl::FboRef textureBuffer, float amount = 0);
	~ChromaticAberrationNode() override;

	void ProcessImpl() override;

protected:
	void SetUniforms() override;

private:
	size_t _amountParameterIndex;
	float _amount;
};

}
}
