#include "Node.h"
#include "Connection.h"

namespace cascade {
namespace node {

Node::Node()
{
	_inputs.reserve(10);
	_outputs.reserve(10);
}

Node::~Node()
{

}

bool Node::ConnectInput(const std::shared_ptr<Node>& other, const std::string & otherOutputName, const std::string & inputName)
{
	size_t inputIndex = GetParameterIndex(ParameterDirection::Input, inputName);
	size_t otherOutputIndex = other->GetParameterIndex(ParameterDirection::Output, otherOutputName);

	std::make_shared<Connection>(other, otherOutputIndex, inputIndex);
	return false;
}

size_t Node::GetParameterIndex(const ParameterDirection direction, const std::string & name)
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
	//This should never happen with the current setup, so maybe this should throw??
	if (_processState == ProcessState::NotProcessed)
	{
		_processState = ProcessState::ProcessStarted;
		ProcessImpl();
	}
}

}
}