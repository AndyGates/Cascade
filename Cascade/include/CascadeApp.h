#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

#include "NodeSystem.h"

using namespace ci::app;
using namespace ci;

namespace cascade
{

namespace node
{
class SourceNode;
class ChromaticAberrationNode;
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

	gl::FboRef			_renderTexture;

	audio::InputDeviceNodeRef		_inputDevice;
	audio::MonitorSpectralNodeRef	_monitorSpectralNode;

	node::NodeSystem _nodeSystem;

	std::shared_ptr<node::SourceNode> _sourceNode;
	std::shared_ptr<node::ChromaticAberrationNode> _postProcessNode;

	std::vector<std::weak_ptr<node::IRenderNode>> _renderNodes;

	float _scale = 1.0f;
	std::vector<float> _spectrumData;
};

}