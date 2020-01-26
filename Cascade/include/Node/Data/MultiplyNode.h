#pragma once

#include "Node/Core/Node.h"

namespace cascade {
namespace node {

template <class T>
class MultiplyNode : public Node
{
public:
	MultiplyNode(T multiplier);

	virtual void ProcessImpl() override;

private:
	float _multiplier;

	size_t _multiplierParameterIndex;
	size_t _valueParameterIndex;
	size_t _inputParameterIndex;
};

template <class T>
MultiplyNode<T>::MultiplyNode(T multiplier) : _multiplier(multiplier)
{
	_inputParameterIndex = AddParameter<T>(ParameterDirection::Input, "Input");
	_multiplierParameterIndex = AddParameter<T>(ParameterDirection::Input, "Multiplier");
	_valueParameterIndex = AddParameter<T>(ParameterDirection::Output, "Value");
}

template <class T>
void MultiplyNode<T>::ProcessImpl()
{
	T input;
	T value;

	if (_inputConnected[_multiplierParameterIndex])
	{
		auto ptr = _inputs[_multiplierParameterIndex]->GetValue<T>();
		_multiplier = *ptr;
	}

	if (_inputConnected[_inputParameterIndex])
	{
		auto ptr = _inputs[_inputParameterIndex]->GetValue<T>();
		input = *ptr;
	}

	value = input * _multiplier;

	_outputs[_valueParameterIndex]->SetValue(value);
}


}
}
