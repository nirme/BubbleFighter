#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include "utils\vector.h"
#include "utils\matrix.h"

#include "AABB.h"
#include "Line.h"
#include "Circle.h"
#include "ConvexPolygon.h"
#include "Pill.h"


namespace Primitives2D
{

	AABB generateAABB(const Circle& circle);
	AABB generateAABB(const ConvexPolygon& polygon);
	AABB generateAABB(const Pill& pill);

}
