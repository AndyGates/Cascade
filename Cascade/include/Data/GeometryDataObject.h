#pragma once
#include "cinder/gl/gl.h"
#include <vector>

namespace cascade {
namespace data {

struct GeometryDataObject
{
	ci::gl::BatchRef		Geometry;
	ci::mat4				Transform;
	ci::Color				Color;
	std::vector<ci::mat4>	Instances;
};

}
}
