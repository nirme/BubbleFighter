#pragma once

#include <cmath>

namespace core
{

	struct Vector2
	{
		union
		{
			float v[2];

			struct
			{
				float x;
				float y;
			};
		};


		inline Vector2(float _s = 0.0f) :
			x(_s), 
			y(_s)
		{};

		inline Vector2(float _x, float _y) :
			x(_x), 
			y(_y)
		{};

		inline Vector2(const Vector2& _v) :
			x(_v.x), 
			y(_v.y)
		{};


		inline Vector2& operator+= (const Vector2& _v)
		{
			x += _v.x;
			y += _v.y;
			return *this;
		};

		inline Vector2& operator-= (const Vector2& _v)
		{
			x -= _v.x;
			y -= _v.y;
			return *this;
		};

		inline Vector2& operator*= (float _s)
		{
			x *= _s;
			y *= _s;
			return *this;
		};

		inline Vector2& operator/= (float _s)
		{
			return *this *= 1.0f / _s;
		};




		inline Vector2 operator+ (const Vector2& _v) const
		{
			return Vector2(*this) += _v;
		};

		inline Vector2 operator- (const Vector2& _v) const
		{
			return Vector2(*this) -= _v;
		};

		inline Vector2 operator* (float _s) const
		{
			return Vector2(*this) *= _s;
		};

		inline Vector2 operator/ (float _s) const
		{
			return Vector2(*this) /= _s;
		};


		inline float length() const
		{
			return std::sqrt(x * x + y * y);
		};

		inline void normalise()
		{
			float lenSq = x * x + y * y;
			if (fabsf(lenSq - 1.0f) > EPSILON)
			{
				*this /= std::sqrt(lenSq);
			}

		};



	};

}
