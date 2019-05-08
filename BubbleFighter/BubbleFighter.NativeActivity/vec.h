#pragma once


class Vec2
{
public:
	union
	{
		float _v[2];
		struct
		{
			float v1, v2;
		};
	};

	Vec2() : v1(0.0f), v2(0.0f)
	{};

	Vec2() : v1(0.0f), v2(0.0f)
	{};



	Vec2 operator * (const Mx2d &m)
	{

	}
};


class Vec3
{
public:
	union
	{
		float _v[3];
		struct
		{
			float v1, v2, v3;
		};
	};
};


class Mx2d
{
public:
	union
	{
		float _m[9];
		struct
		{
			float	m11, m12, m13,
				m21, m22, m23,
				m31, m32, m33;
		};


	};
};


Vec2 transform2d(const Vec2 &v, const Mx2d &m)
{

}
