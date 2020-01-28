#pragma once

#include <memory>
#include <stdexcept>
#include <cassert>

namespace cascade {
namespace node {

class Node;

enum class ParameterDirection
{
	Input,
	Output
};

class Parameter
{
public:

	//Since we cannot properly pass a template argument to a constructor, we can use type deduction to get the type. 
	//This just forces us to pass a parameter to the constructor 
	template <class ParameterType>
	Parameter(ParameterType initialValue) 
		: _parameterValue(std::make_unique<ParameterValue<ParameterType>>(initialValue))
	{
	}

	//Move constructor so this can be used in vector
	Parameter(Parameter&& other)
		: _parameterValue(std::move(other._parameterValue)) 
	{
	}
	
	//Set value via reference to another parameter
	void SetValue(const Parameter& parameter)
	{
		_parameterValue->SetValue(parameter);
	}

	//Set value directly
	template <class ParameterType>
	void SetValue(const ParameterType& value)
	{
		_parameterValue->SetValue(value);
	};

	template <class ParameterType>
	ParameterType* GetValue();

	const std::type_info& GetType() const { return _parameterValue->GetType(); }

	
private:
	//Wrap the value in a wrapper class so that Parameter can be handled independent of the type. 
	class IParameterValue
	{
	public:
		virtual std::type_info const& GetType() const = 0;
		virtual void SetValue(const Parameter& param) = 0;
	};

	template <class ParameterType>
	class ParameterValue : public IParameterValue
	{
	public:
		//Grab the typeinfo once in the constructor
		ParameterValue(ParameterType initial) : _value(initial), _type(typeid(ParameterType)) {};

		void SetValue(const ParameterType& value) { _value = value; };

		void SetValue(const Parameter& param) override
		{
			try {
				ParameterValue<ParameterType>& paramRef = dynamic_cast<ParameterValue<ParameterType>&>(*param._parameterValue);
				SetValue(*paramRef.GetValue());
			}
			catch (const std::bad_cast&)
			{
				//Add some helpful type info to the exception and rethrow
				std::string message = "Could not set parameter value, type mismatch ";
				message += std::string(param.GetType().name());
				message += " -> ";
				message += std::string(GetType().name());

				std::throw_with_nested(std::invalid_argument(message));
			}
		};

		ParameterType* GetValue() { return &_value; }
		const std::type_info& GetType() const { return _type; }

	private:
		ParameterType _value;
		const std::type_info& _type;
	};

	std::unique_ptr<IParameterValue> _parameterValue;
};

template <class ParameterType>
ParameterType* Parameter::GetValue()
{
	ParameterValue<ParameterType>* paramPtr = static_cast<ParameterValue<ParameterType>*>(_parameterValue.get());
	
	assert(paramPtr != nullptr);

	return paramPtr->GetValue();
}

}
}