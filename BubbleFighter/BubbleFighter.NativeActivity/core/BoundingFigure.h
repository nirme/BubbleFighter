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
		const Vec2f* anchoredPosition;
		Vec2f baseOffset;
		float rotation;
		float scale;
		float cashSinO, cashCosO;

	public:

		Bounding2DFigure(const Vec2f* _position, Vec2f _baseOffset, float _rotation, float _scale)
			: anchoredPosition(_position), baseOffset(_baseOffset), scale(_scale)
		{
			setRotation(_rotation);
		};


		virtual Vec2f getPosition() const
		{
			return Vec2f(	anchoredPosition->x + baseOffset.x * cashCosO * scale + baseOffset.y * (-cashSinO) * scale,
							anchoredPosition->y + baseOffset.x * cashSinO * scale + baseOffset.y * cashCosO * scale);
		};

		virtual Vec2f getAnchoredPosition() const
		{
			return *anchoredPosition;
		};

		virtual void setPosition(const Vec2f* point)
		{
			anchoredPosition = point;
		};


		virtual Vec2f getOffset() const
		{
			return Vec2f(	baseOffset.x * cashCosO * scale + baseOffset.y * (-cashSinO) * scale,
							baseOffset.x * cashSinO * scale + baseOffset.y * cashCosO * scale);;
		};

		virtual Vec2f getBaseOffset() const
		{
			return baseOffset;
		};

		virtual void setOffset(const Vec2f& offset)
		{
			baseOffset = offset;
		};


		float getRotation() const
		{
			return rotation;
		};

		virtual void setRotation(float r)
		{
			rotation = r;

			if (rotation > EPSILON)
			{
				cashSinO = sinf(rotation);
				cashCosO = cosf(rotation);
				return;
			}

			cashSinO = 0.f;
			cashCosO = 0.f;
		};


		float getScale() const
		{
			return scale;
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
