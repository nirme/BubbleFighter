#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include "utils\vector.h"
#include "utils\matrix.h"


namespace Primitives2D
{

	class Pill
	{
		union {
			float _v[6];
			struct {
				Vec2f point;
				Vec2f direction;
				float length2;
				float radius;
			};
		};


	public:

		Pill();
		Pill(const Vec2f& _point1, const Vec2f& _point2, float _radius);

		void setup(const Vec2f& _point1, const Vec2f& _point2, float _radius);

		inline const Vec2f& getPoint() const { return point; };
		inline const Vec2f& getDirection() const { return direction; };
		inline float getLenghtHalf() const { return length2; };
		inline float getRadius() const { return radius; };

		inline Vec2f getPoint1() const { return point - (direction * length2); };
		inline Vec2f getPoint2() const { return point + (direction * length2); };
	};


	float distance(const Pill& pill, const Vec2f& point);
	Pill operator*(const Mx3d& mx, const Pill& pill);

}
