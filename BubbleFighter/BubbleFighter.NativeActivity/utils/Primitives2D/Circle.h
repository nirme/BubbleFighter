#pragma once

#include "utils\vector.h"
#include "utils\matrix.h"


namespace Primitives2D
{

	class Circle
	{
		union {
			float _v[3];
			struct {
				Vec2f center;
				float radius;
			};
		};


	public:

		Circle();
		Circle(Vec2f _center, float _radius);
		Circle(const Circle& _circle);
		virtual ~Circle() {};

		inline const Vec2f& getCenter() const { return center; };
		inline const float getRadius() const { return radius; };
	};


	float distance(const Circle& circle, const Vec2f& point);
	Circle operator*(const Mx3d& mx, const Circle& circle);

}
