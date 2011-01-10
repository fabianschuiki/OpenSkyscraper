#include "interpolatedtexturequad.h"

using namespace OSS;
using namespace Engine;


InterpolatedTextureQuad::InterpolatedTextureQuad()
{
	interpolation = 0;
}

void InterpolatedTextureQuad::autogenerateTextureRect(bool x, bool y, double2 offset, double2 scale)
{
	state0.rect = rect;
	state1.rect = rect;
	state0.autogenerateTextureRect(x, y, offset, scale);
	state1.autogenerateTextureRect(x, y, offset, scale);
}

void InterpolatedTextureQuad::draw()
{
	state0.rect = rect;
	state1.rect = rect;
	if (interpolation < 1) {
		state0.alpha = (1 - interpolation);
		state0.draw();
	}
	if (interpolation > 0) {
		state1.alpha = (interpolation);
		state1.draw();
	}
}
