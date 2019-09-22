#include "AABB.h"

namespace Primitives2D
{

	AABB::AABB(float left, float bottom, float right, float top) :
		leftBottom(left, bottom),
		rightTop(right, top)
	{};

	AABB::AABB(const Vec2f& _leftBottom, const Vec2f& _rightTop) :
		leftBottom(_leftBottom),
		rightTop(_rightTop)
	{};

	AABB::AABB(const AABB& rhs) :
		left(rhs.left), bottom(rhs.bottom), right(rhs.right), top(rhs.top)
	{};

	AABB& AABB::operator=(const AABB& rhs)
	{
		left = rhs.left;
		bottom = rhs.bottom;
		right = rhs.right;
		top = rhs.top;
	};

}
