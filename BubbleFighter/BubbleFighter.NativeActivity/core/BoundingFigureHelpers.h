#pragma once

#include <limits>

#include "BoundingFigure.h"
#include "BoundingRectangle.h"
#include "Bounding.h"

#include "Exceptions.h"
#include "vec.h"


namespace App
{
	bool figuresIntersect(const BoundingCircle& f1, const BoundingCircle& f2);
	bool figuresIntersect(const BoundingRectangle& f1, const BoundingRectangle& f2);
	bool figuresIntersect(const BoundingRectangle& f1, const BoundingCircle& f2);
	bool figuresIntersect(const BoundingCircle& f1, const BoundingRectangle& f2);

	float figuresDistance(const BoundingCircle& f1, const BoundingCircle& f2);
	float figuresDistance(const BoundingRectangle& f1, const BoundingRectangle& f2);
	float figuresDistance(const BoundingRectangle& f1, const BoundingCircle& f2);
	float figuresDistance(const BoundingCircle& f1, const BoundingRectangle& f2);
}
