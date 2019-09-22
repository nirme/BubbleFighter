#include "Line.h"


namespace Primitives2D
{

	Line::Line() :
		point(0.f, 0.f),
		direction(1.f, 0.f)
	{};


	Line::Line(const Vec2f& p1, const Vec2f& p2)
	{
		from2Points(p1, p2);
	};


	void Line::fromPointNormal(const Vec2f& p, const Vec2f& _normal)
	{
		fromPointDirection(p, orthogonalCounterClockwise(_normal));
	};


	void Line::fromPointDirection(const Vec2f& p, const Vec2f& _direction)
	{
		point = p;
		direction = normalize(_direction);
	};


	void Line::from2Points(const Vec2f& p1, const Vec2f& p2)
	{
		fromPointDirection(p1, p2 - p1);
	};



	float distance(const Line& line, const Vec2f& point)
	{
		return fabsf(dotProduct(point - line.getPoint(), line.getNormal()));
	};


	Line operator*(const Mx3d& mx, const Line& line)
	{
		return Line(mx * line.getPoint(), mx * (line.getDirection() + line.getPoint()));
	};

}
