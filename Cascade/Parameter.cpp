#include "Node/Core/Parameter.h"
#include "Node/Core/ParameterValue.h"

namespace cascade {
namespace node {

void Parameter::SetValue(const Parameter& parameter)
{
	_parameterValue->SetValue(parameter);
}

}
}