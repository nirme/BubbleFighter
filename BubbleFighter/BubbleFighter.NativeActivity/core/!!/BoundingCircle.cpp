#include "BoundingCircle.h"


namespace App
{

	BoundingCircle::BoundingCircle(const Vec2f& _position, float _r)
		: Bounding2DFigure(_position, 1.0f), r(_r)
	{};


	bool BoundingCircle::contains(const Vec2f& point) const
	{
		float r2 = r * scale;
		r2 *= r2;

		Vec2f dist = position - point;
		dist *= dist;

		return dist.x + dist.y <= r2;
	};


	float BoundingCircle::distance(const Vec2f& point) const
	{
		Vec2f dist = position - point;
		dist *= dist;

		return sqrtf(dist.x + dist.y) - r * scale;
	};


	bool BoundingCircle::intersect(const Bounding2DFigure& figure) const
	{
		return figure.intersect(*this);
	};


	float BoundingCircle::distance(const Bounding2DFigure& figure) const
	{
		return figure.distance(*this);
	};


	bool BoundingCircle::intersect(const BoundingCircle& figure) const
	{
		return figuresIntersect(*this, figure);
	};


	float BoundingCircle::distance(const BoundingCircle& figure) const
	{
		return figuresDistance(*this, figure);
	};


	bool BoundingCircle::intersect(const BoundingRectangle& figure) const
	{
		return figuresIntersect(*this, figure);
	};


	float BoundingCircle::distance(const BoundingRectangle& figure) const
	{
		return figuresDistance(*this, figure);
	};


}
