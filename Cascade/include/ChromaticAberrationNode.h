#pragma once

#include "PostProcessNode.h"

namespace cascade {
namespace node {

class ChromaticAberrationNode : public PostProcessNode
{
public:
	ChromaticAberrationNode(const gl::FboRef& renderTexture);
	~ChromaticAberrationNode() override;

	void Process() override;

protected:
	void SetUniforms() override;

private:
	float _amount;
};

}
}
