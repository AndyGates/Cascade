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
	virtual ~Node() = default;

	//Have index versions as optimisation? 
	//Expose index variables from node
	bool ConnectInput(std::shared_ptr<Node> other, const std::string& otherOutputName, const std::string& inputName);

	size_t GetParameterIndex(const ParameterDirection direction, const std::string& name);
	std::shared_ptr<Parameter> GetParameter(const ParameterDirection direction, size_t index);

	//If the node has no outputs, we can class it as an output node. This means nothing is depending on its process result
	bool IsOutputNode() const { return _outputs.empty(); }

	void Process();
	void ProcessComplete() { _processState = ProcessState::NotProcessed; };

protected:
	std::vector<std::shared_ptr<Parameter>> _inputs;
	std::vector<std::shared_ptr<Parameter>> _outputs;

	std::vector<std::string> _inputNames;
	std::vector<std::string> _outputNames;

	std::vector<bool> _inputConnected;

	//Maybe move this and parameter management to a separate object
	//Like "ParameterSet"?? 
	std::vector<std::shared_ptr<Connection>> _inputConnections;
	
	template <class ParameterType>
	size_t AddParameter(const ParameterDirection& dir, const std::string& name, const ParameterType& initialValue = {});

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
size_t Node::AddParameter(const ParameterDirection& dir, const std::string & name, const ParameterType& initialValue)
{
	//Pass in a default value for whatever type we are assigning to the parameter.
	//This enables type deduction in the constructor. Could pass in a custom initial if necessary
	
	size_t index = -1;

	if (dir == ParameterDirection::Input)
	{
		//emplace_back avoids copy/move constructors 
		_inputs.push_back(std::make_shared<Parameter>(initialValue));
		_inputNames.push_back(name);
		_inputConnected.push_back(false);

		//TODO decide about assertions 
		assert(_inputs.size() == _inputNames.size() && _inputs.size() == _inputConnected.size());
		
		index = _inputs.size() - 1;
	}
	else
	{
		_outputs.push_back(std::make_shared<Parameter>(initialValue));
		_outputNames.push_back(name);

		assert(_outputs.size() == _outputNames.size());

		index = _outputs.size() - 1;
	}

	return index;
}

}
}

