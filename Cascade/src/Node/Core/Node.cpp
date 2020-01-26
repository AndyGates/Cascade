#include "Node/Core/Node.h"
#include "Node/Core/Connection.h"

namespace cascade {
namespace node {

Node::Node()
{
}

Node::~Node() = default;

bool Node::ConnectInput(Node& other, const std::string & otherOutputName, const std::string & inputName)
{
	size_t inputIndex = GetParameterIndex(ParameterDirection::Input, inputName);

	if (!_inputConnected[inputIndex])
	{
		size_t otherOutputIndex = other.GetParameterIndex(ParameterDirection::Output, otherOutputName);
		_inputConnections.push_back(std::make_unique<Connection>(&other, otherOutputIndex, inputIndex));
		_inputConnected[inputIndex] = true;
	}
	return false;
}

size_t Node::GetParameterIndex(const ParameterDirection direction, const std::string & name) const
{
	size_t index = 0;

	auto it = std::find(_inputNames.cbegin(), _inputNames.cend(), name);
	if (it != _inputNames.cend())
	{
		index = std::distance(_inputNames.cbegin(), it);
	}	
	
	return index;
}

void Node::Process()
{
	//Make sure we don't process the same node twice, and don't restart the processing if processing is already taking place (eg. a child node is processing)
	if (_processState == ProcessState::NotProcessed)
	{
		_processState = ProcessState::ProcessStarted;

		for (auto& c : _inputConnections)
		{
			assert(c->OutputNode != nullptr);

			//Make sure we recursively process each input connection before this one
			c->OutputNode->Process();

			auto outParam = c->OutputNode->GetParameter(ParameterDirection::Output, c->OutputIndex);

			//Pass the in the parameter so the value can be recieved in a context where the value type is known
			_inputs[c->InputIndex]->SetValue(outParam);
		}

		ProcessImpl();
	}
}

std::shared_ptr<Parameter> Node::GetParameter(const ParameterDirection direction, size_t index)
{
	//TODO Checks here
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