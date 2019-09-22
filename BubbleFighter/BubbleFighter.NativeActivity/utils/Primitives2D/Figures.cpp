#include "Figures.h"


namespace Primitives2D
{

	AABB generateAABB(const Circle& circle)
	{
		const Vec2f& center = circle.getCenter();
		float r = circle.getRadius();
		return AABB(center.x - r, center.y - r, center.x + r, center.y + r);
	};


	AABB generateAABB(const ConvexPolygon& polygon)
	{
		const std::vector<Line>& sides = polygon.getSides();

		AABB bb(sides[0].getPoint(), sides[0].getPoint());
		for (int i = 1; i < sides.size(); ++i)
		{
			const Vec2f& point = sides[i].getPoint();

			if (point.x < bb.left)			bb.left = point.x;
			else if (point.x > bb.right)	bb.right = point.x;

			if (point.y < bb.bottom)		bb.bottom = point.y;
			else if (point.y > bb.top)		bb.top = point.y;
		}

		return bb;
	};


	AABB generateAABB(const Pill& pill)
	{
		const Vec2f& point = pill.getPoint();
		Vec2f direction = pill.getDirection() * pill.getLenghtHalf();
		float radius = pill.getRadius();
		direction.x = fabsf(direction.x) + radius;
		direction.y = fabsf(direction.y) + radius;

		return AABB(point.x - direction.x, point.y - direction.y, point.x + direction.x, point.y + direction.y);
	};

}
