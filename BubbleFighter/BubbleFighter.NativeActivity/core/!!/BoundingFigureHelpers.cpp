#include "BoundingFigureHelpers.h"


namespace App
{

	bool figuresIntersect(const BoundingCircle& f1, const BoundingCircle& f2)
	{
		Vec2f dist = f1.getPosition() - f2.getPosition();
		float maxDist = f1.getRadius() + f2.getRadius();
		maxDist *= maxDist;

		return maxDist > (dist.x * dist.x + dist.y * dist.y);
	};


	bool figuresIntersect(const BoundingRectangle& f1, const BoundingRectangle& f2)
	{
		return f1.top() >= f2.bottom() &&
			f1.bottom() <= f2.top() &&
			f1.right() >= f2.left() &&
			f1.left() <= f2.right();
	};


	bool figuresIntersect(const BoundingRectangle& f1, const BoundingCircle& f2)
	{
		return f1.distance(f2.getPosition()) <= f2.getRadius();
	};


	bool figuresIntersect(const BoundingCircle& f1, const BoundingRectangle& f2)
	{
		return figuresIntersect(f2, f1);
	};




	float figuresDistance(const BoundingCircle& f1, const BoundingCircle& f2)
	{
		Vec2f dist = f1.getPosition() - f2.getPosition();
		return sqrtf(dist.v1 * dist.v1 + dist.v2 * dist.v2) - f1.getRadius() - f2.getRadius();
	};


	float figuresDistance(const BoundingRectangle& f1, const BoundingRectangle& f2)
	{
		Vec2f diff = f2.getPosition() - f1.getPosition();
		diff.x = fabsf(diff.x);
		diff.y = fabsf(diff.y);

		diff -= f1.getHalfSize();
		diff -= f2.getHalfSize();

		if (diff.x <= 0.f)
		{
			if (diff.y <= 0.f)
				return -std::numeric_limits<float>::infinity();

			return diff.y;
		}

		if (diff.y <= 0.f)
			return diff.x;

		return sqrtf(diff.x * diff.x + diff.y * diff.y);
	};


	float figuresDistance(const BoundingRectangle& f1, const BoundingCircle& f2)
	{
		return f1.distance(f2.getPosition()) - f2.getRadius();
	};


	float figuresDistance(const BoundingCircle& f1, const BoundingRectangle& f2)
	{
		return figuresDistance(f2, f1);
	};

}
