#pragma once

#include <memory>
#include <limits>

#include "core\Exceptions.h"
#include "utils\vector.h"

#include "core\Position2D.h"

#include "utils\Primitives2D\Figures.h"

/*
#include "BoundingCircle.h"
#include "BoundingRectangle.h"
*/


namespace App
{
	class BoundingFigure
	{
	protected:
		Position2D& position;
		unsigned short positionTimeStamp;
		Primitives2D::AABB aabb;

		virtual void _updateFigurePosition() = 0;
		virtual void _updateAABB() = 0;

	public:

		BoundingFigure(Position2D& _position) :
			position(_position), 
			positionTimeStamp(position.getUpdateCounter()-1)
		{};


		void setPosition(Position2D& _position)
		{
			position = _position;
			_updateFigurePosition();
			_updateAABB();
			positionTimeStamp = position.getUpdateCounter();
		};


		inline void update()
		{
			if (positionTimeStamp != position.getUpdateCounter())
			{
				_updateFigurePosition();
				_updateAABB();
				positionTimeStamp = position.getUpdateCounter();
			}
		};


		/*
		virtual bool contains(const Vec2f& point) const = 0;
		virtual float distance(const Vec2f& point) const = 0;

		virtual bool intersect(const BoundingFigure& figure) const = 0;
		virtual float distance(const BoundingFigure& figure) const = 0;

		virtual bool intersect(const BoundingCircle& figure) const = 0;
		virtual float distance(const BoundingCircle& figure) const = 0;

		virtual bool intersect(const BoundingRectangle& figure) const = 0;
		virtual float distance(const BoundingRectangle& figure) const = 0;
		*/

		virtual bool intersect(const BoundingFigure& _figure) const = 0;
		virtual float distance(const BoundingFigure& figure) const = 0;
	};


	template <typename T>
	class BoundingFigureImpl : public BoundingFigure
	{
	protected:

		T baseFigure;
		T transformedFigure;


		virtual void _updateFigurePosition()
		{
			transformedFigure = position.getMatrix() * baseFigure;
		};

		virtual void _updateAABB()
		{
			aabb = generateAABB(transformedFigure);
		};

	public:

		BoundingFigureImpl(Position2D& _position, const T& _figure) :
			BoundingFigure(_position),
			baseFigure(_figure)
		{
			update();
		};


		inline const T& getFigure() const { return transformedFigure; };
		inline const AABB& getAABB() const { return aabb; };

		virtual bool intersect(const BoundingFigure& _figure) const
		{
			return _figure->intersect(*this);
		};

		virtual float distance(const BoundingFigure& figure) const
		{
			return _figure->distance(*this);
		};



		virtual bool intersect(const BoundingFigureImpl<Primitives2D::Circle>& _figure) const
		{
			return BoudingFiguresFunc::figuresIntersecting(*this, _figure);
		};

		virtual float distance(const BoundingFigureImpl<Primitives2D::Circle>& figure) const
		{
			return BoudingFiguresFunc::figuresDistance(*this, _figure);
		};

	};



	typedef BoundingFigureImpl<Primitives2D::Circle> BoundingCircle;
	typedef BoundingFigureImpl<Primitives2D::ConvexPolygon> BoundingConvexPolygon;
	typedef BoundingFigureImpl<Primitives2D::Pill> BoundingPill;


	namespace BoudingFiguresFunc
	{

		bool figuresIntersecting(const BoundingCircle& _figure1, const BoundingCircle& _figure2)
		{
			if (Primitives2D::AABBIntersectionTest(_figure1.getAABB(), _figure2.getAABB()))
			{
				_figure1.getFigure()
			}

			return false;
		};


		float figuresDistance(const BoundingCircle& _figure1, const BoundingCircle& _figure2)
		{};



	};

//	typedef std::unique_ptr<BoundingFigure> UPBounding2DFigure;
}
