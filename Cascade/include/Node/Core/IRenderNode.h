#pragma once

namespace cascade {
namespace node {

class IRenderNode
{
public:
	virtual void Render() = 0;
};

}
}