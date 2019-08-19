#include "Node/Geometry/GeometryInstanceNode.h"

namespace cascade {
namespace node {

GeometryInstanceNode::GeometryInstanceNode()
{
	_geometryInParameterIndex = AddParameter<data::GeometryDataObject>(ParameterDirection::Input, "GeometryIn");
	_geometryOutParameterIndex = AddParameter<data::GeometryDataObject>(ParameterDirection::Output, "GeometryOut");
}

GeometryInstanceNode::~GeometryInstanceNode()
{
}

void GeometryInstanceNode::ProcessImpl()
{
	if (_inputConnected[_geometryInParameterIndex])
	{
		auto ptr = _inputs[_geometryInParameterIndex]->GetValue<data::GeometryDataObject>();
		_geometryData = ptr;

		_outputs[_geometryInParameterIndex]->SetValue(*_geometryData);

		int count = 4;
		float angleDelta = (2.0*M_PI) / static_cast<float>(count);

		for (int i = 0; i < count; i++)
		{
			float angle = angleDelta * static_cast<float>(i);

			ci::vec3 circlePoint(cos(angle), sin(angle), 0.0f);
			gl::scale(0.2f, 0.2f);


			if (_geometryData->Instances.size() < i+1)
			{
				_geometryData->Instances.push_back(ci::mat4());
			}

			_geometryData->Instances[i] *= glm::translate(circlePoint * 2.0f);
			_geometryData->Instances[i] *= glm::scale(ci::vec3(0.2f));
			_geometryData->Instances[i] *= glm::mat4(ci::quat(angle + (M_PI / 2.0), ci::vec3(0,0,1)));
		}
	}
}

}
}