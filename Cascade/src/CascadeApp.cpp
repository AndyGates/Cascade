#include "CascadeApp.h"
#include "SourceNode.h"
#include "ChromaticAberrationNode.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/audio/audio.h"

#include "cinder/ImageIo.h"
#include "cinder/System.h"
#include "cinder/Utilities.h"

#include <vector>

using namespace ci;
using namespace ci::app;

namespace cascade
{

void CascadeApp::mouseDrag(MouseEvent event)
{
}

void CascadeApp::keyDown(KeyEvent event)
{
	gl::Texture2dRef ref = _renderTexture->getColorTexture();
	auto outPath = getHomeDirectory() / "texture.png";
	writeImage(outPath, ref->createSource());
}

void CascadeApp::setupGraphics()
{
	try
	{
		_shader = gl::context()->getStockShader(gl::ShaderDef().color());

	}
	catch (Exception &ex)
	{
		CI_LOG_E("Error loading shader: " << ex.what());
	}

	try
	{
		_geometry = gl::Batch::create(geom::Rect(), _shader);
	}
	catch (Exception &ex)
	{
		CI_LOG_E("Error creating geometry: " << ex.what());
	}


	try
	{
		int height = getWindowHeight();
		int width = getWindowWidth();

		gl::Fbo::Format fmt;
		fmt.samples(16);

		_renderTexture = gl::Fbo::create(width, height, fmt);
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
	_sourceNode = std::make_shared<node::SourceNode>(_monitorSpectralNode);
	_postProcessNode = std::make_shared<node::ChromaticAberrationNode>(_renderTexture);
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
		const gl::ScopedMatrices scopeMat;
		gl::setMatrices(_camera);

		const gl::ScopedFramebuffer fb(_renderTexture);

		gl::clear(Color::gray(0.1f));

		const gl::ScopedColor col(1.0f, 0.5f, 0.25f);

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

			_geometry->draw();
		}
	}
	{
		gl::clear(Color::gray(0.1f));
		_postProcessNode->Render();
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
	_sourceNode->Process();

	_spectrumData = _monitorSpectralNode->getMagSpectrum();
	_scale = _monitorSpectralNode->getVolume();
}

}

CINDER_APP(cascade::CascadeApp, RendererGl(RendererGl::Options().msaa(16)), [](App::Settings* settings) {
	settings->setMultiTouchEnabled(false);
	settings->setWindowSize(1024, 576);
	settings->setTitle("Cascade");
})
