#pragma once

#include "utils\vector.h"
#include "utils\matrix.h"


Mx3d& create2DProjectionMatrix(Mx3d& mx, int width, int height, float scale = 1.f)
{

	float Sx = scale, Sy = scale;

	// set scaling against longer side so we don't extend anything on screens with aspect ratio different than 1:1
	if (width > height)
		Sx *= (float) height / width;
	else if (width < height)
		Sx *= (float) width / height;

	// setup rescaling for provided screen size
	mx.m11 = Sx;
	mx.m22 = Sy;
	mx.m33 = Sy;

	// screen coords are [-0.5,-0.5] (bottom left) to [0.5,0.5] (top right)  and times scales
	mx.m13 = -0.5f * Sx;
	mx.m23 = -0.5f * Sy;

	//clear remaining parts
	mx.m12 = 0.0f;
	mx.m21 = 0.0f;
	mx.m31 = 0.0f;
	mx.m32 = 0.0f;

	return mx;
};