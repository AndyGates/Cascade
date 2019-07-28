#pragma once

#include "Parameter.h"
#include <vector>

namespace cascade { 
namespace node {

class Node
{
public:
	Node();
	virtual ~Node();

	virtual void Process() = 0;

protected:
	
	template <class ParameterType>
	void AddParameter(const ParameterDirection dir, const std::string& name);

	void RemoveParameter();
	
	std::vector<std::shared_ptr<Parameter>> _inputCollection;
	std::vector<std::shared_ptr<Parameter>> _outputCollection;
};

template<class ParameterType>
void Node::AddParameter(const ParameterDirection dir, const std::string & name)
{
	ParameterType initial = {};
	auto newParam = std::make_shared<Parameter>(initial);

	if (dir == ParameterDirection::Input)
	{
		_inputCollection.push_back(newParam);
	}
	else
	{
		_outputCollection.push_back(newParam);
	}
}

}
}

