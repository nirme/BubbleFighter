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

	Vec2(float _v1, float _v2) : v1(_v1), v2(_v2)
	{};

	Vec2(const Vec2& _v) : v1(_v.v1), v2(_v.v2)
	{};

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

	Vec3() : v1(0.0f), v2(0.0f), v3(0.0f)
	{};

	Vec3(float _v1, float _v2, float _v3) : v1(_v1), v2(_v2), v3(_v3)
	{};

	Vec3(const Vec3& _v) : v1(_v.v1), v2(_v.v2), v3(_v.v3)
	{};
};



class Mx2d
{
public:
	union
	{
		float _m[4];
		struct
		{
			float	m11, m12,
					m21, m22;
		};
	};


	Mx2d() : m11(0), m12(0), m21(0), m22(0)
	{};

	Mx2d(float _m11, float _m12, float _m21, float _m22) : m11(_m11), m12(_m12), m21(_m21), m22(_m22)
	{};

	Mx2d(const Mx2d& _m) : m11(_m.m11), m12(_m.m12), m21(_m.m21), m22(_m.m22)
	{};

	inline static Mx2d& identity(Mx2d& _m)
	{
		_m.m11 = 1.f;
		_m.m12 = 0.f;
		_m.m21 = 0.f;
		_m.m22 = 1.f;
		return _m;
	};

	inline static Mx2d identity()
	{
		return identity(Mx2d());
	};

	inline void identity()
	{
		identity(*this);
	};



	inline Vec2& mul(const Vec2& _v, Vec2& _out)
	{
		_out.v1 = m11 * _v.v1 + m12 * _v.v2;
		_out.v2 = m21 * _v.v1 + m22 * _v.v2;
		return _out;
	}

	inline Vec2 mul(const Vec2& _v)
	{
		return mul(_v, Vec2());
	}

	inline Vec2 operator *(const Vec2& _v)
	{
		return mul(_v, Vec2());
	}



	inline Mx2d& mul(const Mx2d& _m, Mx2d& _out)
	{
		_out.m11 = m11 * _m.m11 + m12 * _m.m21;
		_out.m12 = m11 * _m.m12 + m12 * _m.m22;
		_out.m21 = m21 * _m.m11 + m22 * _m.m21;
		_out.m22 = m21 * _m.m12 + m22 * _m.m22;
		return _out;
	}

	inline Mx2d mul(const Mx2d& _m)
	{
		return mul(_m, Mx2d());
	}

	inline Mx2d operator *(Mx2d _m)
	{
		return mul(_m, Mx2d());
	}

};


class Mx3d
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



	Mx3d() : m11(0), m12(0), m13(0), m21(0), m22(0), m23(0), m31(0), m32(0), m33(0)
	{};

	Mx3d(	float _m11, float _m12, float _m13, 
			float _m21, float _m22, float _m23,
			float _m31, float _m32, float _m33) :	m11(_m11), m12(_m12), m13(_m13), 
													m21(_m21), m22(_m22), m23(_m23),
													m31(_m31), m32(_m32), m33(_m33)
	{};

	Mx3d(const Mx3d& _m) : m11(_m.m11), m12(_m.m12), m13(_m.m13), m21(_m.m21), m22(_m.m22), m23(_m.m23), m31(_m.m31), m32(_m.m32), m33(_m.m33)
	{};

	inline static Mx3d& identity(Mx3d& _m)
	{
		_m.m11 = 1.f;
		_m.m12 = 0.f;
		_m.m13 = 0.f;
		_m.m21 = 0.f;
		_m.m22 = 1.f;
		_m.m23 = 0.f;
		_m.m31 = 0.f;
		_m.m32 = 0.f;
		_m.m33 = 1.f;
		return _m;
	};

	inline static Mx3d identity()
	{
		return identity(Mx3d());
	};

	inline void identity()
	{
		identity(*this);
	};



	inline Vec3& mul(const Vec3& _v, Vec3& _out)
	{
		_out.v1 = m11 * _v.v1 + m12 * _v.v2 + m13 * _v.v3;
		_out.v2 = m21 * _v.v1 + m22 * _v.v2 + m23 * _v.v3;
		_out.v3 = m31 * _v.v1 + m32 * _v.v2 + m33 * _v.v3;

		return _out;
	}

	inline Vec3 mul(const Vec3& _v)
	{
		return mul(_v, Vec3());
	}

	inline Vec3 operator *(const Vec3& _v)
	{
		return mul(_v, Vec3());
	}


	inline Mx3d& mul(const Mx3d& _m, Mx3d& _out)
	{
		_out.m11 = m11 * _m.m11 + m12 * _m.m21 + m13 * _m.m31;
		_out.m12 = m11 * _m.m12 + m12 * _m.m22 + m13 * _m.m32;
		_out.m13 = m11 * _m.m13 + m12 * _m.m23 + m13 * _m.m33;

		_out.m21 = m21 * _m.m11 + m22 * _m.m21 + m23 * _m.m31;
		_out.m22 = m21 * _m.m12 + m22 * _m.m22 + m23 * _m.m32;
		_out.m23 = m21 * _m.m13 + m22 * _m.m23 + m23 * _m.m33;

		_out.m31 = m31 * _m.m11 + m32 * _m.m21 + m33 * _m.m31;
		_out.m32 = m31 * _m.m12 + m32 * _m.m22 + m33 * _m.m32;
		_out.m33 = m31 * _m.m13 + m32 * _m.m23 + m33 * _m.m33;

		return _out;
	}

	inline Mx3d mul(const Mx3d& _m)
	{
		return mul(_m, Mx3d());
	}

	inline Mx3d operator *(Mx3d _m)
	{
		return mul(_m, Mx3d());
	}


};


Vec2 transform2d(const Vec2 &v, const Mx2d &m)
{

}
