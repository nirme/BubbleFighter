#include "BoundingRectangle.h"


namespace App
{

	BoundingRectangle::BoundingRectangle(const Vec2f& _position, const Vec2f& _size)
		: Bounding2DFigure(_position, 1.0f), halfSize(_size * 0.5f)
	{};


	bool BoundingRectangle::contains(const Vec2f& point) const
	{
		return fabsf(point.x - position.x) <= (halfSize.x * scale) &&
			fabsf(point.y - position.y) <= (halfSize.y * scale);
	};


	float BoundingRectangle::distance(const Vec2f& point) const
	{
		Vec2f diff = position - point;
		diff.x = fabsf(diff.x);
		diff.y = fabsf(diff.y);
		diff -= (halfSize * scale);

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


	bool BoundingRectangle::intersect(const Bounding2DFigure& figure) const
	{
		return figure.intersect(*this);
	};


	float BoundingRectangle::distance(const Bounding2DFigure& figure) const
	{
		return figure.intersect(*this);
	};


	bool BoundingRectangle::intersect(const BoundingCircle& figure) const
	{
		return figuresIntersect(*this, figure);
	};


	float BoundingRectangle::distance(const BoundingCircle& figure) const
	{
		return figuresDistance(*this, figure);
	};


	bool BoundingRectangle::intersect(const BoundingRectangle& figure) const
	{
		return figuresIntersect(*this, figure);
	};


	float BoundingRectangle::distance(const BoundingRectangle& figure) const
	{
		return figuresDistance(*this, figure);
	};

}
