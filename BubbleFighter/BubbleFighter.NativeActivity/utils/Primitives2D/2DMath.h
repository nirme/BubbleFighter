#pragma once


#include "AABB.h"
#include "Line.h"
#include "Circle.h"
#include "ConvexPolygon.h"
#include "Pill.h"


namespace Primitives2D
{
	bool figureIntersection(const Circle& _figure1, const Circle& _figure2)
	{
		Vec2f dist = _figure2.getCenter() - _figure1.getCenter();
		float r1 = _figure2.getRadius();
		float r2 = _figure2.getRadius();
		
		return (dist.x * dist.x + dist.y * dist.y) <= (r1 * r1 + r2 * r2);
	};

	bool figureIntersection(const ConvexPolygon& _figure1, const ConvexPolygon& _figure2)
	{

	};

	bool figureIntersection(const Pill& _figure1, const Pill& _figure2);

	bool figureIntersection(const Line& _figure1, const Line& _figure2);




	bool figureIntersection(const Circle& _figure1, const ConvexPolygon& _figure2);
	bool figureIntersection(const ConvexPolygon& _figure1, const Circle& _figure2) { return figureIntersection(_figure2, _figure1); };

	bool figureIntersection(const Circle& _figure1, const Pill& _figure2);
	bool figureIntersection(const Pill& _figure1, const Circle& _figure2) { return figureIntersection(_figure2, _figure1); };

	bool figureIntersection(const Circle& _figure1, const Line& _figure2);
	bool figureIntersection(const Line& _figure1, const Circle& _figure2) { return figureIntersection(_figure2, _figure1); };

	bool figureIntersection(const ConvexPolygon& _figure1, const Pill& _figure2);
	bool figureIntersection(const Pill& _figure1, const ConvexPolygon& _figure2) { return figureIntersection(_figure2, _figure1); };

	bool figureIntersection(const ConvexPolygon& _figure1, const Line& _figure2);
	bool figureIntersection(const Line& _figure1, const ConvexPolygon& _figure2) { return figureIntersection(_figure2, _figure1); };

	bool figureIntersection(const Pill& _figure1, const Line& _figure2);
	bool figureIntersection(const Line& _figure1, const Pill& _figure2) { return figureIntersection(_figure2, _figure1); };





}
