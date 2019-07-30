#pragma once

#include "Parameter.h"
#include <vector>

namespace cascade { 
namespace node {

struct Connection;

class Node
{
public:
	Node();
	virtual ~Node();

	//Have index versions as optimisation? 
	//Expose index variables from node
	bool ConnectInput(const std::shared_ptr<Node>& other, const std::string& otherOutputName, const std::string& inputName);

	size_t GetParameterIndex(const ParameterDirection direction, const std::string& name);

	//If the node has no outputs, we can class it as an output node. This means nothing is depending on its process result
	bool IsOutputNode() const { return _outputs.empty(); }

	void Process();
	void ProcessComplete() { _processState = ProcessState::NotProcessed; };

protected:
	std::vector<Parameter> _inputs;
	std::vector<Parameter> _outputs;

	std::vector<std::string> _inputNames;
	std::vector<std::string> _outputNames;

	//Maybe move this and parameter management to a separate object
	//Like "ParameterSet"?? 
	std::vector<std::shared_ptr<Connection>> _inputConnections;
	
	template <class ParameterType>
	size_t AddParameter(const ParameterDirection& dir, const std::string& name);

	virtual void ProcessImpl() = 0;

private:
	enum class ProcessState
	{
		NotProcessed,
		ProcessStarted,
		ProcessComplete
	};

	ProcessState _processState = ProcessState::NotProcessed;
};

template<class ParameterType>
size_t Node::AddParameter(const ParameterDirection& dir, const std::string & name)
{
	//Default value for whatever tyoe we are assigning to the parameter.
	//This enables type deduction in the constructor. Could pass in a custom initial if necessary
	ParameterType initial = {};
	size_t index = -1;

	if (dir == ParameterDirection::Input)
	{
		//emplace_back avoids copy/move constructors 
		_inputs.emplace_back(initial);
		_inputNames.push_back(name);

		index = _inputs.size() - 1;
	}
	else
	{
		_outputs.emplace_back(initial);
		_outputNames.push_back(name);

		index = _outputs.size() - 1;
	}

	return index;
}

}
}

