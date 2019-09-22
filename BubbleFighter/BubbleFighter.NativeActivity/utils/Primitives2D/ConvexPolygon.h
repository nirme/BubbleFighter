#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include <limits>
#include <vector>

#include "utils\vector.h"
#include "utils\matrix.h"

#include "Line.h"


namespace Primitives2D
{

	class ConvexPolygon
	{
		std::vector<Line> sides; //contains points with normals

	protected:

		void buildFromPoints(const std::vector<Vec2f>& _points);
		void buildFromLines(std::vector<Line>& _sides);

		static bool isFigureConvex(const std::vector<Line>& _sides);
		static bool isFigureConvex(const std::vector<Vec2f>& _points);

		static std::vector<Vec2f> regularPolygonPoints(unsigned int _n, float _r);

	public:


		ConvexPolygon();
		ConvexPolygon(unsigned int _n, float _r = 1.f);
		virtual ~ConvexPolygon() {};

		bool fromPoints(std::vector<Vec2f>& _points);
		bool fromLines(std::vector<Line>& _sides);

		inline const std::vector<Line>& getSides() const { return sides; };
	};


	float distance(const ConvexPolygon& polygon, const Vec2f& point);
	ConvexPolygon operator*(const Mx3d& mx, const ConvexPolygon& polygon);

}
