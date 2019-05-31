#pragma once

#include <list>
#include <memory>

#include "Exceptions.h"
#include "BoundingFigure.h"
#include "BoundingFigureHelpers.h"


namespace App
{

	class BoundingComplexFigure : public Bounding2DFigure
	{
	protected:

		typedef std::list<UPBounding2DFigure> BoundingFigureList;
		BoundingFigureList figureList;


	public:

		BoundingComplexFigure(const Vec2f& _position, float _scale);

		const BoundingFigureList& getList();

		void attachBoundingFigure(Bounding2DFigure* figure);
		virtual void setPosition(const Vec2f& point);
		virtual void setScale(float _scale);

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
