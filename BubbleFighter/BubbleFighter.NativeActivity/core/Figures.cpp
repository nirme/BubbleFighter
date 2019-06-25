#include "utils\Figures.h"


namespace Primitives2D
{

	Line::Line() :
		point(0.f, 0.f),
		normal(0.f, 1.f)
	{};


	Line::Line(const Vec2f& p1, const Vec2f& p2)
	{
		from2Points(p1, p2);
	};


	void Line::fromPointNormal(const Vec2f& p, const Vec2f& _normal)
	{
		point = p;
		normal = normalize(_normal);
	};


	void Line::fromPointDirection(const Vec2f& p, const Vec2f& direction)
	{
		point = p;
		normal = normalize(orthogonalCounterClockwise(direction));
	};


	void Line::from2Points(const Vec2f& p1, const Vec2f& p2)
	{
		fromPointDirection(p1, p2 - p1);
	};


	float Line::distance(const Vec2f& p) const
	{
		return dotProduct(p - point, normal);
	};


	Line& Line::transform(const Mx3d& mx)
	{
		point = mx * point;
		Vec2f newDirection = normalize(mx * getDirection());
		normal = Vec2f(newDirection.y, -newDirection.x);

		return *this;
	};




	Circle::Circle() :
		center(0.f, 0.f),
		radius(0.f)
	{};


	Circle::Circle(Vec2f _center, float _radius) :
		center(_center),
		radius(_radius)
	{};


	float Circle::distance(const Vec2f& p) const
	{
		float dist = length(p - center);
		return dist - radius;
	};


	Circle& Circle::transform(const Mx3d& mx)
	{
		//Vec2f r(center.x + radius, center.y);

		float r2 = sqrtf(radius * radius * 0.5f);
		Vec2f r = center + r2;

		center = mx * center;
		radius = length((mx * r) - center);

		return *this;
	};




	void ConvexPolygon::precalcLines()
	{
		sides.resize(points.size());
		sides.shrink_to_fit();

		for (int i = 0; i < points.size() - 1; ++i)
			sides[i].from2Points(points[i], points[i + 1]);

		sides[points.size() - 1].from2Points(points[points.size() - 1], points[0]);
	};


	void ConvexPolygon::buildFromPoints(std::vector<Vec2f>& _points)
	{
		points.swap(_points);
		points.shrink_to_fit();
		precalcLines();
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
		points(0),
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


	float ConvexPolygon::distance(const Vec2f& p) const
	{
		float dist = std::numeric_limits<float>::max();
		bool isInside = true;
		float lineDist;

		for (int i = 0; i < sides.size(); ++i)
		{
			lineDist = sides[i].distance(p);

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


	ConvexPolygon& ConvexPolygon::transform(const Mx3d& mx)
	{
		for (int i = 0; i < points.size(); ++i)
			points[i] = mx * points[i];

		precalcLines();
		return *this;
	};




	Line operator*(const Mx2d& mx, const Line& line)
	{
		return Line(mx * line.getPoint(), mx * (line.getDirection() + line.getPoint()));
	};


	Circle operator*(const Mx2d& mx, const Circle& circle)
	{
		Vec2f center = mx * circle.getCenter();
		Vec2f offPoint = mx * (circle.getCenter() + sqrtf(circle.getRadius() * circle.getRadius() * 0.5f));
		return Circle(center, length(offPoint - center));
	};


	ConvexPolygon operator*(const Mx2d& mx, const ConvexPolygon& polygon)
	{
		std::vector<Vec2f> points = polygon.getPoints();
		for (int i = 0; i < points.size(); ++i)
			points[i] = mx * points[i];

		return ConvexPolygon().fromPoints(points);
	};






}
