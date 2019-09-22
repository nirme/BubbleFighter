#include "ConvexPolygon.h"

namespace Primitives2D
{

	void ConvexPolygon::buildFromPoints(const std::vector<Vec2f>& _points)
	{
		sides.resize(_points.size());
		sides.shrink_to_fit();

		for (int i = 0; i < _points.size() - 1; ++i)
			sides[i].fromPointDirection(_points[i], _points[i + 1] - _points[i]);

		int i = _points.size() - 1;
		sides[i].fromPointDirection(_points[i], _points[0] - _points[i]);
	};


	void ConvexPolygon::buildFromLines(std::vector<Line>& _sides)
	{
		sides.swap(_sides);
		sides.shrink_to_fit();
	};


	bool ConvexPolygon::isFigureConvex(const std::vector<Line>& _sides)
	{
		for (int i = 0; i < _sides.size(); ++i)
			if (dotProduct(_sides[i].getNormal(), _sides[(i + 1) % _sides.size()].getDirection()) >= 0.f)
				return false;

		return true;
	};


	bool ConvexPolygon::isFigureConvex(const std::vector<Vec2f>& _points)
	{
		Vec2f directionNext = _points[1] - _points[0];
		Vec2f normalPrev;

		for (int i = 1; i < _points.size() + 1; ++i)
		{
			normalPrev = orthogonalCounterClockwise(directionNext);
			directionNext = _points[(i + 1) % _points.size()] - _points[i % _points.size()];

			if (dotProduct(normalPrev, directionNext) > 0)
				return false;
		}

		return true;
	};


	std::vector<Vec2f> ConvexPolygon::regularPolygonPoints(unsigned int _n, float _r)
	{
		std::vector<Vec2f> pts(_n);

		float pointHalfRotation = M_PI / _n;
		float rotation;

		for (int i = 0; i < _n; ++i)
		{
			rotation = i * pointHalfRotation;
			if (_n % 2 == 0)
				rotation += pointHalfRotation;

			pts[i].x = sinf(rotation) * _r;
			pts[i].y = cosf(rotation) * _r;
		}

		return pts;
	};


	ConvexPolygon::ConvexPolygon() :
		sides(0)
	{};


	ConvexPolygon::ConvexPolygon(unsigned int _n, float _r)
	{
		std::vector<Vec2f> pts = regularPolygonPoints(_n, _r);
		buildFromPoints(pts);
	};


	bool ConvexPolygon::fromPoints(std::vector<Vec2f>& _points)
	{
		if (!isFigureConvex(_points))
			return false;

		buildFromPoints(_points);
		return true;
	};


	bool ConvexPolygon::fromLines(std::vector<Line>& _sides)
	{
		if (!isFigureConvex(_sides))
			return false;

		buildFromLines(_sides);
		return true;
	};




	float distance(const ConvexPolygon& polygon, const Vec2f& point)
	{
		const std::vector<Line>& sides = polygon.getSides();
		float dist = std::numeric_limits<float>::max();
		bool isInside = true;
		float lineDist;

		for (int i = 0; i < sides.size(); ++i)
		{
			lineDist = distance(sides[i], point);

			if (lineDist > 0.f)
			{
				isInside = false;
				if (dist <= 0.f || lineDist < dist)
					dist = lineDist;
			}
			else if (isInside == true && lineDist > dist)
				dist = lineDist;
		}

		return dist;
	};


	ConvexPolygon operator*(const Mx3d& mx, const ConvexPolygon& polygon)
	{
		std::vector<Line> sides = polygon.getSides();
		for (int i = 0; i < sides.size(); ++i)
			sides[i] = mx * sides[i];

		return ConvexPolygon().fromLines(sides);
	};

}
