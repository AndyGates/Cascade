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
		: _parameterValue(std::make_unique < ParameterValue<ParameterType>>())
	{}
	
	~Parameter() { }

	template <class ParameterType>
	void SetValue(const ParameterType& value);

	template <class ParameterType>
	const std::shared_ptr<ParameterType>& GetValue() const;
	
private:
	class IParameterValue
	{
		virtual std::type_info const& GetType() const = 0;
	};

	template <class ParameterType>
	class ParameterValue : public IParameterValue
	{
	public:
		//Grab the typeinfo once in the constructor
		ParameterValue() : _value(), _type(typeid(ParameterType)) {};
		~ParameterValue() {};

		void SetValue(const ParameterType& value) { _value = value; };

		//Return shared_ptr to value, needs to be a pointer as the value can be null (Can it? should it?) 
		//Is this too slow?
		const std::shared_ptr<ParameterType>& GetValue() const { return std::make_shared<ParameterType>(_value); }
		const std::type_info& GetType() const { return _type; }

	private:
		ParameterType _value;
		std::type_info const& _type;
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
		throw std::invalid_argument("Could not set parameter value, type mismatch");
	}
};

template <class ParameterType>
const std::shared_ptr<ParameterType>& Parameter::GetValue() const
{
	try
	{
		ParameterValue<ParameterType>& paramRef = dynamic_cast<ParameterValue<ParameterType>&>(*_parameterValue);
		return paramRef.GetValue();
	}
	catch (const std::bad_cast&)
	{
		throw std::invalid_argument("Could not get parameter value, type mismatch");
	}
}

}
}