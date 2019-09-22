#pragma once

#include "utils\vector.h"


namespace Primitives2D
{

	struct AABB
	{
		union {
			Vec2f _p[2];
			struct {
				Vec2f leftBottom;
				Vec2f rightTop;
			};
			struct {
				float left;
				float bottom;
				float right;
				float top;
			};
		};

		AABB(float left, float bottom, float right, float top);
		AABB(const Vec2f& _leftBottom, const Vec2f& _rightTop);
		AABB(const AABB& rhs);

		AABB& operator=(const AABB& rhs);
	};


	inline bool AABBIntersectionTest(const AABB& b1, const AABB& b2)
	{
		return	b1.left < b2.right &&
			b1.right > b2.left &&
			b1.bottom < b2.top &&
			b1.top > b2.bottom;
	};

}
