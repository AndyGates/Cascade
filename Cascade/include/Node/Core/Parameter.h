#pragma once

#include <memory>
#include <stdexcept>

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
	void SetValue(const Parameter& parameter);

	//Set value directly
	template <class ParameterType>
	void SetValue(const ParameterType& value);

	template <class ParameterType>
	ParameterType* GetValue();
	
private:
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
			ParameterValue<ParameterType>& paramRef = dynamic_cast<ParameterValue<ParameterType>&>(*param._parameterValue);
			try {
				_value = *paramRef.GetValue();
			}
			catch (const std::bad_cast&)
			{
				//TODO Throw custom exception here
				throw std::invalid_argument("Could not set parameter value, type mismatch");
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
void Parameter::SetValue(const ParameterType& value)
{
	try
	{
		ParameterValue<ParameterType>& paramRef = dynamic_cast<ParameterValue<ParameterType>&>(*_parameterValue);
		return paramRef.SetValue(value);
	}
	catch (const std::bad_cast&)
	{
		//TODO Throw custom exception here
		throw std::invalid_argument("Could not set parameter value, type mismatch");
	}
};

template <class ParameterType>
ParameterType* Parameter::GetValue()
{
	try
	{
		ParameterValue<ParameterType>& paramRef = dynamic_cast<ParameterValue<ParameterType>&>(*_parameterValue);
		return paramRef.GetValue();
	}
	catch (const std::bad_cast&)
	{
		//TODO Throw custom exception here
		throw std::invalid_argument("Could not get parameter value, type mismatch");
	}
}

}
}