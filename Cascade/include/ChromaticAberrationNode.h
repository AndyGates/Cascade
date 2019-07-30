#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class ChromaticAberrationNode : public PostProcessNode
{
public:
	ChromaticAberrationNode(const gl::FboRef& renderTexture);
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
