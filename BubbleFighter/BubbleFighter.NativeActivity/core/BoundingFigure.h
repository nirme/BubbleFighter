#pragma once

#include <memory>
#include <limits>

#include "Exceptions.h"
#include "vec.h"

#include "BoundingCircle.h"
#include "BoundingRectangle.h"



namespace App
{
	class BoundingCircle;
	class BoundingRectangle;


	class Bounding2DFigure
	{
	protected:
		Vec2f position;
		float scale;

	public:
		Bounding2DFigure(const Vec2f& _position, float _scale)
			: position(_position), scale(_scale)
		{};;


		inline const Vec2f& getPosition() const
		{
			return position;
		};

		inline float getScale() const
		{
			return scale;
		};


		virtual void setPosition(const Vec2f& point)
		{
			position = point;
		};
		
		virtual void setScale(float _scale)
		{
			scale = _scale;
		};


		virtual bool contains(const Vec2f& point) const					{ throw NotImplementedException(); };
		virtual float distance(const Vec2f& point) const				{ throw NotImplementedException(); };

		virtual bool intersect(const Bounding2DFigure& figure) const	{ throw NotImplementedException(); };
		virtual float distance(const Bounding2DFigure& figure) const	{ throw NotImplementedException(); };

		virtual bool intersect(const BoundingCircle& figure) const		{ throw NotImplementedException(); };
		virtual float distance(const BoundingCircle& figure) const		{ throw NotImplementedException(); };

		virtual bool intersect(const BoundingRectangle& figure) const	{ throw NotImplementedException(); };
		virtual float distance(const BoundingRectangle& figure) const	{ throw NotImplementedException(); };
	};

	
	
	typedef std::unique_ptr<Bounding2DFigure> UPBounding2DFigure;


}
