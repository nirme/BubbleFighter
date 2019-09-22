#pragma once

#include "Exceptions.h"
#include "BoundingFigure.h"
#include "BoundingFigureHelpers.h"


namespace App
{

	class BoundingCircle : public Bounding2DFigure
	{
	private:
		float r;

	public:
		BoundingCircle(const Vec2f& _position, float _r);

		inline float getRadius() const
		{
			return r * scale;
		};

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
