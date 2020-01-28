#include "Node/Core/Node.h"
#include "Node/Core/Connection.h"
#include "cinder/Log.h"

namespace cascade {
namespace node {

Node::Node()
{
}

Node::~Node() = default;

bool Node::ConnectInput(Node& other, const std::string & otherOutputName, const std::string & inputName)
{
	//Make sure the parameters exist and are compatible before connecting
	size_t inputIndex;
	bool hasInput = GetParameterIndex(ParameterDirection::Input, inputName, inputIndex);

	if (hasInput && !_inputConnected[inputIndex])
	{
		size_t otherOutputIndex;
		bool otherHasOutput = other.GetParameterIndex(ParameterDirection::Output, otherOutputName, otherOutputIndex);

		//Make sure the other output exists and matches the type
		if (otherHasOutput && other.CanConnectToOutput(otherOutputIndex, _inputs[inputIndex].GetType()))
		{
			_inputConnections.push_back(Connection(&other, otherOutputIndex, inputIndex));
			_inputConnected[inputIndex] = true;
			return true;
		}
	}

	std::string err = "Failed to connect " + inputName + " -> " + otherOutputName;
	CI_LOG_E(err);

	return false;
}

bool Node::CanConnectToOutput(size_t outputIndex, const type_info& typeinfo)
{
	//Make sure the output exists and the types match
	return outputIndex < _outputs.size() && typeinfo == _outputs[outputIndex].GetType();
}

bool Node::GetParameterIndex(ParameterDirection direction, const std::string & name, size_t& index)
{
	auto nameVec = direction == ParameterDirection::Input ? _inputNames : _outputNames;

	auto it = std::find(nameVec.cbegin(), nameVec.cend(), name);
	if (it != nameVec.cend())
	{
		index = std::distance(nameVec.cbegin(), it);
		return true;
	}		

	return false;
}

void Node::Process()
{
	//Make sure we don't process the same node twice, and don't restart the processing if processing is already taking place (eg. a child node is processing)
	if (_processState == ProcessState::NotProcessed)
	{
		_processState = ProcessState::ProcessStarted;

		for (auto& c : _inputConnections)
		{
			assert(c.OutputNode != nullptr);

			//Make sure we recursively process each input connection before this one
			c.OutputNode->Process();

			const Parameter& outParam = c.OutputNode->GetParameter(ParameterDirection::Output, c.OutputIndex);

			//Pass the in the parameter so the value can be recieved in a context where the value type is known
			_inputs[c.InputIndex].SetValue(outParam);
		}

		ProcessImpl();

		_processState = ProcessState::ProcessComplete;
	}
}

const Parameter& Node::GetParameter(ParameterDirection direction, size_t index)
{
	if (direction == ParameterDirection::Input)
	{
		return _inputs[index];
	}
	else
	{
		return _outputs[index];
	}
}

}
}