#pragma once

#include <memory>
#include <stdexcept>
#include "cinder/Log.h"

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
	
	~Parameter() { }


	void SetValue(std::shared_ptr<Parameter> parameter)
	{
		_parameterValue->SetValue(*parameter);
	};

	template <class ParameterType>
	void SetValue(const ParameterType& value);

	template <class ParameterType>
	const std::shared_ptr<ParameterType> GetValue() const;
	
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
		
		//TODO move out? all these classes?
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

		//Return shared_ptr to value, needs to be a pointer as the value can be null (Can it? should it?) 
		//Is this too slow?
		const std::shared_ptr<ParameterType> GetValue() const { return std::make_shared<ParameterType>(_value); }
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
const std::shared_ptr<ParameterType> Parameter::GetValue() const
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