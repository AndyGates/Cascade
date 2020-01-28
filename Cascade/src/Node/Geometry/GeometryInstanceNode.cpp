#include "Node/Geometry/GeometryInstanceNode.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

namespace cascade {
namespace node {

GeometryInstanceNode::GeometryInstanceNode(uint32_t instanceCount, float radius) : 
	_instanceCount(instanceCount),
	_radius(radius),
	_angleCumulative(0)
{
	_geometryInParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Input, IN_GEOMETRY);
	_tempInParameterIndex = AddParameter<float>(ParameterDirection::Input, IN_TEMP);

	_geometryOutParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Output, OUT_GEOMETRY);
}

void GeometryInstanceNode::ProcessImpl()
{
	if (_inputConnected[_geometryInParameterIndex])
	{
		_geometryData = *_inputs[_geometryInParameterIndex].GetValue<std::shared_ptr<data::GeometryDataObject>>();

		_outputs[_geometryInParameterIndex].SetValue(_geometryData);

		float twoPi = M_PI * 2.0f;

		float angleDelta = twoPi / static_cast<float>(_instanceCount);
		float timeIn = *_inputs[_tempInParameterIndex].GetValue<float>();

		_angleCumulative = fmod(_angleCumulative + timeIn, twoPi) ;

		for (uint32_t i = 0; i < _instanceCount; i++)
		{
			float angle = (angleDelta * static_cast<float>(i)) + _angleCumulative;

			ci::vec3 circlePoint(cos(angle), sin(angle), 0.0f);

			if (_geometryData->Instances.size() < i+1)
			{
				_geometryData->Instances.push_back(glm::mat4());
			}

			glm::mat4 instanceTransform;

			instanceTransform *= glm::translate(circlePoint * _radius);
			instanceTransform *= glm::rotate(angle, ci::vec3(0.0f, 0.0f, 1.0f));
			instanceTransform *= glm::scale(ci::vec3(0.2f, 0.2f, 1.0f));

			_geometryData->Instances[i] = instanceTransform;
			_geometryData->Color = ci::Color::hex(0xFF652F);
		}
	}
}

}
}