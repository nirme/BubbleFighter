#include "Pill.h"

namespace Primitives2D
{
	Pill::Pill() :
		point(0.f, 0.f),
		direction(1.f, 0.f),
		length2(0.f),
		radius(0.f)
	{};

	Pill::Pill(const Vec2f& _point1, const Vec2f& _point2, float _radius) :
		point(_point1 + ((_point2 - _point1) *0.5f)),
		direction(normalize(_point2 - _point1)),
		length2(length(_point2 - _point1) * 0.5f),
		radius(_radius)
	{};


	void Pill::setup(const Vec2f& _point1, const Vec2f& _point2, float _radius)
	{
		point = _point1 + ((_point2 - _point1) *0.5f);
		direction = normalize(_point2 - _point1);
		length2 = length(_point2 - _point1) * 0.5f;
		radius = _radius;
	};

	float distance(const Pill& pill, const Vec2f& point)
	{
		float distanceOffCenter = dotProduct(point - pill.getPoint(), pill.getDirection());

		if (distanceOffCenter < -pill.getLenghtHalf())
			return length(point - pill.getPoint1()) - pill.getRadius();

		if (distanceOffCenter > pill.getLenghtHalf())
			return length(point - pill.getPoint2()) - pill.getRadius();

		return fabsf(dotProduct(point - pill.getPoint(), orthogonalClockwise(pill.getDirection()))) - pill.getRadius();
	};

	Pill operator*(const Mx3d& mx, const Pill& pill)
	{
		float r2 = pill.getRadius();
		r2 = sqrtf(0.5 * r2 * r2);
		return Pill(mx * pill.getPoint1(), mx * pill.getPoint2(), length(mx * Vec2f(r2, r2)));
	};

}