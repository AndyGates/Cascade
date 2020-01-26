#include "CascadeApp.h"
#include "Node/Geometry/GeometrySourceNode.h"
#include "Node/Geometry/GeometryInstanceNode.h"
#include "Node/RenderNode.h"
#include "Node/Source/SourceNode.h"
#include "Node/Effect/ChromaticAberrationNode.h"
#include "Node/Effect/VignetteNode.h"
#include "Node/Effect/TiltShiftNode.h"
#include "Node/Data/MultiplyNode.h"
#include "Node/Effect/KaleidoscopeNode.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/audio/audio.h"

#include "cinder/ImageIo.h"
#include "cinder/System.h"
#include "cinder/Utilities.h"

#include <vector>

#include <chrono>

#include "Data/GeometryDataObject.h"

using namespace std::chrono;

using namespace ci;
using namespace ci::app;

namespace cascade
{

void CascadeApp::mouseDrag(MouseEvent event)
{
}

void CascadeApp::keyDown(KeyEvent event)
{
	gl::Texture2dRef ref = _primaryRenderTexture->getColorTexture();
	auto outPath = getHomeDirectory() / "texture.png";
	writeImage(outPath, ref->createSource());

	ref = _secondaryRenderTexture->getColorTexture();
	outPath = getHomeDirectory() / "texture2.png";
	writeImage(outPath, ref->createSource());
}

void CascadeApp::setupGraphics()
{
	try
	{
		int height = getWindowHeight();
		int width = getWindowWidth();

		gl::Fbo::Format fmt;
		fmt.samples(16);

		_primaryRenderTexture = gl::Fbo::create(width, height, fmt);
		_secondaryRenderTexture = gl::Fbo::create(width, height, fmt);
	}
	catch (Exception &ex)
	{
		CI_LOG_E("Error creating render texture: " << ex.what());
	}
}

void CascadeApp::setupAudio()
{
	auto ctx = audio::Context::master();
	auto device = audio::Device::findDeviceByName("CABLE Output (VB-Audio Virtual Cable)");

	_inputDevice = ctx->createInputDeviceNode(device);

	auto monitorFormat = audio::MonitorSpectralNode::Format().windowSize(512);
	_monitorSpectralNode = ctx->makeNode(new audio::MonitorSpectralNode(monitorFormat));

	_inputDevice >> _monitorSpectralNode;
	_inputDevice->enable();

	ctx->enable();
}

void CascadeApp::setupNodes()
{
	//Source nodes
	auto sourceNode = std::make_shared<node::SourceNode>(_monitorSpectralNode);

	//Data nodes
	auto multiply = std::make_shared<node::MultiplyNode<float>>(0.01f);

	
	//Geometry
	auto outerGeom = std::make_shared<node::GeometrySourceNode>();
	auto outerGeomInstance = std::make_shared<node::GeometryInstanceNode>();
	auto outerRender = std::make_shared<node::RenderNode>(_camera, _primaryRenderTexture);

	auto innerGeom = std::make_shared<node::GeometrySourceNode>();
	auto innerRender = std::make_shared<node::RenderNode>(_camera, _primaryRenderTexture);

	//Post Process
	auto tiltShift = std::make_shared<node::TiltShiftNode>(_secondaryRenderTexture, _primaryRenderTexture, 0.004f, 0.2f);
	auto chromaticAberrationNode = std::make_shared<node::ChromaticAberrationNode>(nullptr, _primaryRenderTexture);
	
	_nodeSystem.AddNode(sourceNode);
	_nodeSystem.AddNode(multiply);

	_nodeSystem.AddNode(outerGeom);
	_nodeSystem.AddNode(outerGeomInstance);
	_nodeSystem.AddNode(outerRender);

	_nodeSystem.AddNode(innerGeom);
	_nodeSystem.AddNode(innerRender);
	
	_nodeSystem.AddNode(tiltShift);
	_nodeSystem.AddNode(chromaticAberrationNode);

	outerGeomInstance->ConnectInput(outerGeom, "Geometry", "GeometryIn");
	outerRender->ConnectInput(outerGeomInstance, "GeometryOut", "Geometry");

	innerRender->ConnectInput(innerGeom, "Geometry", "Geometry");

	multiply->ConnectInput(sourceNode, "Volume", "Input");
	chromaticAberrationNode->ConnectInput(multiply, "Value", "Amount");
}

void CascadeApp::setup()
{
	setupGraphics();
	setupAudio();
	setupNodes();
}

void CascadeApp::draw()
{
	{
		const gl::ScopedFramebuffer fb(_primaryRenderTexture);
		gl::clear(Color::hex(0x00171F));

		/*
		const gl::ScopedMatrices scopeMat;
		gl::setMatrices(_camera);

		const gl::ScopedFramebuffer fb(_primaryRenderTexture);

		gl::clear(Color::hex(0x00171F));

		const gl::ScopedColor col(Color::hex(0x00A8E8));

		int num = 32;
		float angleDelta = (2.0*M_PI) / static_cast<float>(num);

		float spectrumCount = static_cast<float>(_spectrumData.size());
		float spectrumStart = spectrumCount * 0.1;
		float spectrumEnd = spectrumCount * 0.9;

		float spectrumDelta = (spectrumCount*0.8f) / static_cast<float>(num);

		for (int i = 0; i < num; i++)
		{
			const gl::ScopedModelMatrix mat;
			float angle = angleDelta * static_cast<float>(i);

			int spectrumIndex = static_cast<int>(spectrumStart + (spectrumDelta * i));

			vec2 circlePoint(cos(angle), sin(angle));
			float spectrumAmount = _spectrumData[spectrumIndex] * 1000.0f;

			gl::translate(circlePoint * 2.0f);
			gl::rotate(angle + (M_PI / 2.0));
			gl::scale(0.2f, 0.2f + (0.1f*spectrumAmount));

			_geom->GetParameter(node::ParameterDirection::Output, 0)->GetValue<data::GeometryDataObject>()->Geometry->draw();
		}

		float cumulativeRot = -fmod(app::getElapsedSeconds(), 1.0f) * (_monitorSpectralNode->getVolume() / 100.0);
		_rot += cumulativeRot;
		_rot = fmod(_rot, M_PI*2.0f);

		for (int i = 0; i < num; i++)
		{
			const gl::ScopedColor col(Color::hex(0xEE5622));

			const gl::ScopedModelMatrix mat;
			float angle = (angleDelta * static_cast<float>(i)) + _rot;
			
			vec2 circlePoint(cos(angle), sin(angle));

			gl::translate(circlePoint * 4.0f);
			gl::rotate(angle + (M_PI / 2.0));
			gl::scale(0.4f, 0.4f);

			_geometry->draw();
		}
		*/
	}

	{
		gl::clear(Color(1.0f, 0.0f, 1.0f));
		_nodeSystem.Render();
	}
}

void CascadeApp::resize()
{
	int height = getWindowHeight();
	int width = getWindowWidth();
	float aspect = (float)height / (float)width;

	float referenceWith = 10.0f;
	float calculatedHeight = referenceWith * aspect;

	_camera.setOrtho(-referenceWith, referenceWith, -calculatedHeight, calculatedHeight, -100, 100);

	/*
	try
	{

		gl::Fbo::Format fmt;
		fmt.samples(16);
		_renderTexture = gl::Fbo::create(width, height, fmt);
	}
	catch (Exception &ex)
	{
		CI_LOG_E("Error creating render texture: " << ex.what());
	}
	*/
}

void CascadeApp::update()
{
	_nodeSystem.Process();

	_spectrumData = _monitorSpectralNode->getMagSpectrum();
	_scale = _monitorSpectralNode->getVolume();
}

}

CINDER_APP(cascade::CascadeApp, RendererGl(RendererGl::Options().msaa(16)), [](App::Settings* settings) {
	settings->setMultiTouchEnabled(false);
	settings->setWindowSize(1366, 768);
	settings->setTitle("Cascade");
})
