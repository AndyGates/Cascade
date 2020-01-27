#include "CascadeApp.h"
#include "Node/Geometry/GeometrySourceNode.h"
#include "Node/Geometry/GeometryInstanceNode.h"
#include "Node/Core/RenderNode.h"
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
	int height = getWindowHeight();
	int width = getWindowWidth();

	gl::Fbo::Format fmt;
	fmt.samples(16);

	try
	{
		_primaryRenderTexture = gl::Fbo::create(width, height, fmt);
		_secondaryRenderTexture = gl::Fbo::create(width, height, fmt);
	}
	catch (Exception &ex)
	{
		CI_LOG_EXCEPTION("Error creating render texture: ", ex);
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
	//Move to a factory in the future? 

	//Source nodes
	auto sourceNode = std::make_unique<node::SourceNode>(_monitorSpectralNode);

	//Data nodes
	auto multiply = std::make_unique<node::MultiplyNode<float>>(0.02f);
	
	//Geometry
	auto outerGeom = std::make_unique<node::GeometrySourceNode>();
	auto outerGeomInstance = std::make_unique<node::GeometryInstanceNode>();
	auto outerRender = std::make_unique<node::RenderNode>(_camera, _primaryRenderTexture);

	auto innerGeom = std::make_unique<node::GeometrySourceNode>();
	auto innerRender = std::make_unique<node::RenderNode>(_camera, _primaryRenderTexture);

	//Post Process
	auto tiltShift = std::make_unique<node::TiltShiftNode>(_secondaryRenderTexture, _primaryRenderTexture, 0.004f, 0.2f);
	auto chromaticAberrationNode = std::make_unique<node::ChromaticAberrationNode>(nullptr, _primaryRenderTexture);


	//Connect nodes
	//Outer squares
	outerGeomInstance->ConnectInput(*outerGeom, node::GeometrySourceNode::OUT_GEOMETRY, node::GeometryInstanceNode::IN_GEOMETRY);
	outerRender->ConnectInput(*outerGeomInstance, node::GeometryInstanceNode::OUT_GEOMETRY, node::RenderNode::IN_GEOMETRY);

	//Inner square
	innerRender->ConnectInput(*innerGeom, node::GeometrySourceNode::OUT_GEOMETRY, node::RenderNode::IN_GEOMETRY);

	//Post Effects
	multiply->ConnectInput(*sourceNode, node::SourceNode::OUT_VOLUME, node::MultiplyNode<float>::IN_INPUT);
	chromaticAberrationNode->ConnectInput(*multiply, node::MultiplyNode<float>::OUT_VALUE, node::ChromaticAberrationNode::IN_AMOUNT);


	//Add nodes to node system
	_nodeSystem.AddNode(std::move(sourceNode));
	_nodeSystem.AddNode(std::move(multiply));

	_nodeSystem.AddNode(std::move(outerGeom));
	_nodeSystem.AddNode(std::move(outerGeomInstance));
	_nodeSystem.AddNode(std::move(outerRender));

	_nodeSystem.AddNode(std::move(innerGeom));
	_nodeSystem.AddNode(std::move(innerRender));
	
	_nodeSystem.AddNode(std::move(tiltShift));
	_nodeSystem.AddNode(std::move(chromaticAberrationNode));
}

void CascadeApp::setup()
{
	setupGraphics();
	setupAudio();

	try
	{
		setupNodes();
	}
	catch (const std::exception& e)
	{
		/*
			For now just log out if any node construction fails.
			Currently this would be unable to be recovered from, but in the future we could fallback to something sensible 
		*/
		CI_LOG_EXCEPTION("Failed to construct nodes! ", e);
	}
}

void CascadeApp::draw()
{
	{
		const gl::ScopedFramebuffer fb(_primaryRenderTexture);
		gl::clear(Color::hex(0x00171F));
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
