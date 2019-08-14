#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

#include "Node/Core/NodeSystem.h"

using namespace ci::app;
using namespace ci;

namespace cascade
{

namespace node
{
class IRenderNode;
}

class CascadeApp : public App {
public:
	void mouseDrag(MouseEvent event) override;
	void keyDown(KeyEvent event) override;

	void setupNodes();
	void setupGraphics();
	void setupAudio();

	void setup() override;
	void draw() override;
	void resize() override;
	void update() override;

private:

	gl::BatchRef		_geometry;
	gl::GlslProgRef		_shader;
	CameraOrtho			_camera;

	gl::FboRef			_primaryRenderTexture;
	//second render texture for post process "ping-pong"
	gl::FboRef			_secondaryRenderTexture;

	audio::InputDeviceNodeRef		_inputDevice;
	audio::MonitorSpectralNodeRef	_monitorSpectralNode;

	node::NodeSystem _nodeSystem;
	std::vector<std::weak_ptr<node::IRenderNode>> _renderNodes;

	float _scale = 1.0f;
	std::vector<float> _spectrumData;
};

}