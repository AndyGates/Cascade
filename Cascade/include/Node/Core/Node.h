#pragma once

#include "Parameter.h"
#include <vector>
#include <cassert>

namespace cascade { 
namespace node {

struct Connection;

class Node
{
public:
	Node();
	virtual ~Node();

	//This is done using string 'names' currently with the idea of making it clearer to define in a file in the future.
	//This could be done with only indices as an optimisation
	bool ConnectInput(Node& other, const std::string& otherOutputName, const std::string& inputName);

	bool GetParameterIndex(ParameterDirection direction, const std::string& name, size_t& index);

	//If the node has no outputs, we can class it as an output node. This means nothing is depending on its process result
	bool IsOutputNode() const { return _outputs.empty(); }

	void Process();
	void ProcessComplete() { _processState = ProcessState::NotProcessed; };

protected:
	virtual void ProcessImpl() = 0;

	template <class ParameterType>
	size_t AddParameter(ParameterDirection dir, const std::string& name, const ParameterType& initialValue = {});
	const Parameter& GetParameter(ParameterDirection direction, size_t index);

	bool CanConnectToOutput(size_t outputIndex, const type_info& typeinfo);

	//These could be consolidated into structures
	std::vector<Parameter> _inputs;
	std::vector<Parameter> _outputs;

	std::vector<std::string> _inputNames;
	std::vector<std::string> _outputNames;

	std::vector<bool> _inputConnected;

	std::vector<Connection> _inputConnections;

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
size_t Node::AddParameter(ParameterDirection dir, const std::string & name, const ParameterType& initialValue)
{
	//Pass in a default value for whatever type we are assigning to the parameter.
	//This enables type deduction in the constructor. Could pass in a custom initial if necessary
	
	size_t index;

	if (dir == ParameterDirection::Input)
	{
		_inputs.push_back(Parameter(initialValue));
		_inputNames.push_back(name);
		_inputConnected.push_back(false);

		assert(_inputs.size() == _inputNames.size() && _inputs.size() == _inputConnected.size());
		
		index = _inputs.size() - 1;
	}
	else
	{
		_outputs.push_back(Parameter(initialValue));
		_outputNames.push_back(name);

		assert(_outputs.size() == _outputNames.size());

		index = _outputs.size() - 1;
	}

	return index;
}

}
}

