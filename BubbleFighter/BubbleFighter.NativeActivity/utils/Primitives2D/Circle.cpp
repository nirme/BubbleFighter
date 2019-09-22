#include "Circle.h"


namespace Primitives2D
{

	Circle::Circle() :
		center(0.f, 0.f),
		radius(0.f)
	{};


	Circle::Circle(Vec2f _center, float _radius) :
		center(_center),
		radius(_radius)
	{};


	Circle::Circle(const Circle& _circle) :
		center(_circle.center),
		radius(_circle.radius)
	{};



	float distance(const Circle& circle, const Vec2f& point)
	{
		float dist = length(point - circle.getCenter());
		return dist - circle.getRadius();
	};


	Circle operator*(const Mx3d& mx, const Circle& circle)
	{
		Vec2f center = mx * circle.getCenter();
		Vec2f offPoint = mx * (circle.getCenter() + sqrtf(circle.getRadius() * circle.getRadius() * 0.5f));
		return Circle(center, length(offPoint - center));
	};

}
