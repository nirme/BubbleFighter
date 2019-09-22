#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include <utility>

#include "utils\vector.h"
#include "utils\matrix.h"


namespace Primitives2D
{

	class Line
	{
		union {
			Vec2f _d[2];
			struct {
				Vec2f point;
				Vec2f direction;
			};
		};


	public:

		Line();
		Line(const Vec2f& p1, const Vec2f& p2);
		virtual ~Line() {};

		void fromPointNormal(const Vec2f& p, const Vec2f& _normal);
		void fromPointDirection(const Vec2f& p, const Vec2f& _direction);
		void from2Points(const Vec2f& p1, const Vec2f& p2);

		inline const Vec2f& getPoint() const { return point; };
		inline const Vec2f& getDirection() const { return direction; };
		//normal being clockwise to direction
		inline Vec2f getNormal() const { return Vec2f(direction.y, -direction.x); };
	};


	float distance(const Line& line, const Vec2f& point);
	Line operator*(const Mx3d& mx, const Line& line);


	std::pair<bool, Vec2f> linesIntersection(const Line& l1, const Line& l2)
	{
		// dot = |u|*|v|*cos(O)
		//cos(90deg) = 0
		if (dotProduct(l1.getDirection(), l2.getNormal()) < EPSILON)
			return std::make_pair(false, Vec2f());
		

	};

}
