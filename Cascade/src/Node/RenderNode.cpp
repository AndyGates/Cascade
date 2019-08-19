#include "Node/RenderNode.h"

namespace cascade {
namespace node {

RenderNode::RenderNode(const ci::CameraOrtho& camera, ci::gl::FboRef renderTarget) :
	_camera(camera),
	_renderTarget(renderTarget)
{
	_geometryParameterIndex = AddParameter<data::GeometryDataObject>(ParameterDirection::Input, "Geometry");
}

RenderNode::~RenderNode()
{
}

void RenderNode::ProcessImpl()
{
	if (_inputConnected[_geometryParameterIndex])
	{
		auto ptr = _inputs[_geometryParameterIndex]->GetValue<data::GeometryDataObject>();
		_geometryData = ptr;
	}
}

void RenderNode::Render()
{
	if (_geometryData != nullptr)
	{
		const gl::ScopedMatrices scopeMat;
		gl::setMatrices(_camera);

		const gl::ScopedFramebuffer fb(_renderTarget);
		const gl::ScopedColor col(ci::Color::hex(0x00A8E8));
		
		const gl::ScopedModelMatrix mat;
		gl::setModelMatrix(_geometryData->Transform);

		_geometryData->Geometry->draw();

		/*
		int num = 32;
		float angleDelta = (2.0*M_PI) / static_cast<float>(num);

		for (int i = 0; i < num; i++)
		{
			const gl::ScopedModelMatrix mat;
			float angle = angleDelta * static_cast<float>(i);

			ci::vec2 circlePoint(cos(angle), sin(angle));

			gl::translate(circlePoint * 2.0f);
			gl::rotate(angle + (M_PI / 2.0));
			gl::scale(0.2f, 0.2f);

			_geometryData->Geometry->draw();
		}
		*/
	}
}

}
}