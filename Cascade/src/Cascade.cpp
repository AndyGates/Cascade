#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;

// We'll create a new Cinder Application by deriving from the App class.
class Cascade : public App {
public:
	void mouseDrag(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void setup() override;
	void draw() override;
	void resize() override;

private:

	gl::BatchRef		_geometry;
	gl::GlslProgRef		_shader;
	CameraOrtho			_camera;
	
};

void Cascade::mouseDrag(MouseEvent event)
{
}

void Cascade::keyDown(KeyEvent event)
{
}

void Cascade::setup()
{
	try
	{
		_shader = gl::context()->getStockShader(gl::ShaderDef().color());
	}
	catch(Exception &ex)
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
}

void Cascade::draw()
{
	gl::clear(Color::gray(0.1f));

	gl::ScopedColor col(1.0f, 0.5f, 0.25f);

	gl::ScopedModelMatrix mat;
	gl::rotate(getElapsedSeconds() / 5.0);

	_geometry->draw();
}

void Cascade::resize()
{
	int height = getWindowHeight();
	int width = getWindowWidth();
	float aspect = (float)height / (float)width;

	float referenceWith = 10.0f;
	float calculatedHeight = referenceWith * aspect;

	_camera.setOrtho(-referenceWith, referenceWith, -calculatedHeight, calculatedHeight, -100, 100);
	gl::setMatrices(_camera);
}

CINDER_APP(Cascade, RendererGl(RendererGl::Options().msaa(16)), [] (App::Settings* settings) {
	settings->setMultiTouchEnabled(false); 
	settings->setWindowSize(1024, 576);
})
