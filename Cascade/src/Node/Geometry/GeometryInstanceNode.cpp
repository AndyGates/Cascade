#include "Node/Geometry/GeometryInstanceNode.h"
#include "cinder/app/App.h"

namespace cascade {
namespace node {

GeometryInstanceNode::GeometryInstanceNode()
{
	_geometryInParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Input, "GeometryIn");
	_geometryOutParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Output, "GeometryOut");
}

GeometryInstanceNode::~GeometryInstanceNode()
{
}

void GeometryInstanceNode::ProcessImpl()
{
	if (_inputConnected[_geometryInParameterIndex])
	{
		auto ptr = _inputs[_geometryInParameterIndex]->GetValue<std::shared_ptr<data::GeometryDataObject>>();
		_geometryData = *ptr;

		_outputs[_geometryInParameterIndex]->SetValue(_geometryData);

		int count = 32;
		float angleDelta = (2.0*M_PI) / static_cast<float>(count);

		for (int i = 0; i < count; i++)
		{
			float angle = (angleDelta * static_cast<float>(i)) + (ci::app::getElapsedSeconds() / 5.0f);

			ci::vec3 circlePoint(cos(angle), sin(angle), 0.0f);

			if (_geometryData->Instances.size() < i+1)
			{
				_geometryData->Instances.push_back(glm::mat4());
			}

			glm::mat4 instanceTransform;

			instanceTransform *= glm::translate(circlePoint * 3.0f);
			instanceTransform *= glm::rotate(angle, ci::vec3(0.0f, 0.0f, 1.0f));
			instanceTransform *= glm::scale(ci::vec3(1.0f, 0.05f, 1.0f));

			 _geometryData->Instances[i] = instanceTransform;
		}
	}
}

}
}