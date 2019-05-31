#include "BoundingCompound.h"

namespace App
{

	BoundingComplexFigure::BoundingComplexFigure(const Vec2f& _position, float _scale)
		: Bounding2DFigure(_position, _scale)
	{};


	const BoundingComplexFigure::BoundingFigureList& BoundingComplexFigure::getList()
	{
		return figureList;
	};

	void BoundingComplexFigure::attachBoundingFigure(Bounding2DFigure* figure)
	{
		figure->setPosition(position + figure->getPosition());
		figure->setScale(figure->getScale() * scale);
		figureList.push_back(UPBounding2DFigure(figure));
	};


	void BoundingComplexFigure::setPosition(const Vec2f& point)
	{
		Vec2f diff = point - position;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			(*it)->setPosition((*it)->getPosition() + diff);
	};


	void BoundingComplexFigure::setScale(float _scale)
	{
		scale = _scale;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			(*it)->setScale(scale);
	};



	bool BoundingComplexFigure::contains(const Vec2f& point) const
	{
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			if ((*it)->contains(point))
				return true;

		return false;
	};


	float BoundingComplexFigure::distance(const Vec2f& point) const
	{
		float dist = std::numeric_limits<float>::max(), tmp;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
		{
			tmp = (*it)->distance(point);
			dist = tmp < dist ? tmp : dist;
			if (dist < 0.f)
				return dist;
		}

		return dist;
	};


	bool BoundingComplexFigure::intersect(const Bounding2DFigure& figure) const
	{
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			if ((*it)->intersect(figure))
				return true;

		return false;
	};


	float BoundingComplexFigure::distance(const Bounding2DFigure& figure) const
	{
		float dist = std::numeric_limits<float>::max(), tmp;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
		{
			tmp = (*it)->distance(figure);
			dist = tmp < dist ? tmp : dist;
			if (dist < 0.f)
				return dist;
		}

		return dist;
	};


	bool BoundingComplexFigure::intersect(const BoundingCircle& figure) const
	{
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			if ((*it)->intersect(figure))
				return true;

		return false;
	};


	float BoundingComplexFigure::distance(const BoundingCircle& figure) const
	{
		float dist = std::numeric_limits<float>::max(), tmp;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
		{
			tmp = (*it)->distance(figure);
			dist = tmp < dist ? tmp : dist;
			if (dist < 0.f)
				return dist;
		}

		return dist;
	};


	bool BoundingComplexFigure::intersect(const BoundingRectangle& figure) const
	{
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
			if ((*it)->intersect(figure))
				return true;

		return false;
	};


	float BoundingComplexFigure::distance(const BoundingRectangle& figure) const
	{
		float dist = std::numeric_limits<float>::max(), tmp;
		for (auto it = figureList.begin(); it != figureList.end(); ++it)
		{
			tmp = (*it)->distance(figure);
			dist = tmp < dist ? tmp : dist;
			if (dist < 0.f)
				return dist;
		}

		return dist;
	};

}
