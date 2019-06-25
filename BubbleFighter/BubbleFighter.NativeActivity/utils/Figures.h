#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include <limits>
#include <vector>

#include "utils\vector.h"
#include "utils\matrix.h"


namespace Primitives2D
{

	class Line
	{
		union {
			Vec2f _d[2];
			struct {
				Vec2f point;
				Vec2f normal;
			};
		};


	public:

		Line();
		Line(const Vec2f& p1, const Vec2f& p2);
		virtual ~Line() {};

		void fromPointNormal(const Vec2f& p, const Vec2f& _normal);
		void fromPointDirection(const Vec2f& p, const Vec2f& direction);
		void from2Points(const Vec2f& p1, const Vec2f& p2);

		inline const Vec2f& getPoint() const	{	return point;	};
		inline const Vec2f& getNormal() const	{	return normal;	};
		inline Vec2f getDirection() const		{	return Vec2f(-normal.y, normal.x);	};

		float distance(const Vec2f& p) const;
		Line& transform(const Mx3d& mx);
	};



	class Circle
	{
		union {
			float _v[3];
			struct {
				Vec2f center;
				float radius;
			};
		};


	public:

		Circle();
		Circle(Vec2f _center, float _radius);
		virtual ~Circle() {};

		inline const Vec2f& getCenter() const	{	return center;	};
		inline const float getRadius() const	{	return radius;	};

		float distance(const Vec2f& p) const;
		Circle& transform(const Mx3d& mx);
	};



	class ConvexPolygon
	{
		std::vector<Vec2f> points;
		std::vector<Line> sides; //precalc from points


	protected:

		void precalcLines();
		void buildFromPoints(std::vector<Vec2f>& _points);

		static bool isFigureConvex(const std::vector<Vec2f>& _points);
		static std::vector<Vec2f> regularPolygonPoints(unsigned int _n, float _r);


	public:

		ConvexPolygon();
		ConvexPolygon(unsigned int _n, float _r = 1.f);
		virtual ~ConvexPolygon() {};
		
		bool fromPoints(std::vector<Vec2f>& _points);

		inline const std::vector<Vec2f>& getPoints() const	{	return points;	};

		float distance(const Vec2f& p) const;
		ConvexPolygon& transform(const Mx3d& mx);
	};




	Line operator*(const Mx2d& mx, const Line& line);
	Circle operator*(const Mx2d& mx, const Circle& circle);
	ConvexPolygon operator*(const Mx2d& mx, const ConvexPolygon& polygon);

}