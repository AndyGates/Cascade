#include "Node/RenderNode.h"

namespace cascade {
namespace node {

RenderNode::RenderNode(const ci::CameraOrtho& camera, ci::gl::FboRef renderTarget) :
	_camera(camera),
	_renderTarget(renderTarget)
{
	_geometryParameterIndex = AddParameter<std::shared_ptr<data::GeometryDataObject>>(ParameterDirection::Input, "Geometry");
}

RenderNode::~RenderNode()
{
}

void RenderNode::ProcessImpl()
{
	if (_inputConnected[_geometryParameterIndex])
	{
		auto ptr = _inputs[_geometryParameterIndex].GetValue<std::shared_ptr<data::GeometryDataObject>>();
		_geometryData = *ptr;
	}
}

void RenderNode::Render()
{
	if (_geometryData != nullptr)
	{
		const gl::ScopedMatrices scopeMat;
		gl::setMatrices(_camera);

		const gl::ScopedFramebuffer fb(_renderTarget);
		const gl::ScopedColor col(_geometryData->Color);
		
		if (_geometryData->Instances.size() > 0)
		{
			
			for (glm::mat4 instance : _geometryData->Instances)
			{
				RenderInstance(instance);
			}
		}
		else
		{
			RenderInstance();
		}

	}
}

void RenderNode::RenderInstance(glm::mat4 transform)
{
	const gl::ScopedModelMatrix mat;
	
	//Combine the base object transform with the instance transform
	glm::mat4 compTransform = _geometryData->Transform * transform;
	gl::setModelMatrix(compTransform);
	
	_geometryData->Geometry->draw();
}

}
}