#pragma once

#include "Exceptions.h"
#include "BoundingFigure.h"
#include "BoundingFigureHelpers.h"


namespace App
{

	class BoundingRectangle : public Bounding2DFigure
	{
	private:
		Vec2f halfSize;

	public:
		BoundingRectangle(const Vec2f& _position, const Vec2f& _size);

		inline float top() const
		{
			return position.y + halfSize.y * scale;
		};

		inline float bottom() const
		{
			return position.y - halfSize.y * scale;
		};

		inline float right() const
		{
			return position.x + halfSize.x * scale;
		};

		inline float left() const
		{
			return position.x - halfSize.x * scale;
		};

		inline Vec2f getHalfSize() const { return halfSize * scale; };

		virtual bool contains(const Vec2f& point) const;
		virtual float distance(const Vec2f& point) const;

		virtual bool intersect(const Bounding2DFigure& figure) const;
		virtual float distance(const Bounding2DFigure& figure) const;
		virtual bool intersect(const BoundingCircle& figure) const;
		virtual float distance(const BoundingCircle& figure) const;
		virtual bool intersect(const BoundingRectangle& figure) const;
		virtual float distance(const BoundingRectangle& figure) const;

	};

}
