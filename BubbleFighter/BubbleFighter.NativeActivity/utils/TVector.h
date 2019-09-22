#pragma once


template <class T>
struct TVec2
{
	union
	{
		T _v[2];
		struct
		{
			T v1, v2;
		};
		struct
		{
			T x, y;
		};
	};

	TVec2()
	{};

	TVec2(T _v1, T _v2)
		: v1(_v1), v2(_v2)
	{};

	TVec2(const TVec2& v)
		: v1(v.v1), v2(v.v2)
	{};


	TVec2 operator- () const
	{
		return TVec2(-v1, -v2);
	};

	TVec2 operator+ (const TVec2& vec) const
	{
		return TVec2(v1 + vec.v1, v2 + vec.v2);
	};

	TVec2 operator- (const TVec2& vec) const
	{
		return TVec2(v1 - vec.v1, v2 - vec.v2);
	};

	TVec2 operator* (const TVec2& vec) const
	{
		return TVec2(v1 * vec.v1, v2 * vec.v2);
	};

	TVec2 operator/ (const TVec2& vec) const
	{
		return TVec2(v1 / vec.v1, v2 / vec.v2);
	};


	TVec2 operator+ (T v) const
	{
		return TVec2(v1 + v, v2 + v);
	};

	TVec2 operator- (T v) const
	{
		return TVec2(v1 - v, v2 - v);
	};

	TVec2 operator* (T v) const
	{
		return TVec2(v1 * v, v2 * v);
	};

	TVec2 operator/ (T v) const
	{
		return TVec2(v1 / v, v2 / v);
	};


	TVec2& operator+= (const TVec2& vec)
	{
		v1 += vec.v1;
		v2 += vec.v2;
		return *this;
	};

	TVec2& operator-= (const TVec2& vec)
	{
		v1 -= vec.v1;
		v2 -= vec.v2;
		return *this;
	};

	TVec2& operator*= (const TVec2& vec)
	{
		v1 *= vec.v1;
		v2 *= vec.v2;
		return *this;
	};

	TVec2& operator/= (const TVec2& vec)
	{
		v1 /= vec.v1;
		v2 /= vec.v2;
		return *this;
	};


	TVec2& operator+= (T v)
	{
		v1 += v;
		v2 += v;
		return *this;
	};

	TVec2& operator-= (T v)
	{
		v1 -= v;
		v2 -= v;
		return *this;
	};

	TVec2& operator*= (T v)
	{
		v1 *= v;
		v2 *= v;
		return *this;
	};

	TVec2& operator/= (T v)
	{
		v1 /= v;
		v2 /= v;
		return *this;
	};
};




template <class T>
class TVec3
{
public:
	union
	{
		T _v[3];
		struct
		{
			T v1, v2, v3;
		};
		struct
		{
			T x, y, w;
		};
	};

	TVec3()
	{};

	TVec3(T _v1, T _v2, T _v3)
		: v1(_v1), v2(_v2), v3(_v3)
	{};


	TVec3(const TVec3& v)
		: v1(v.v1), v2(v.v2), v3(v.v3)
	{};


	TVec3 operator- () const
	{
		return TVec3(-v1, -v2, -v3);
	};

	TVec3 operator+ (const TVec3& vec) const
	{
		return TVec3(v1 + vec.v1, v2 + vec.v2, v3 + vec.v3);
	};

	TVec3 operator- (const TVec3& vec) const
	{
		return TVec3(v1 - vec.v1, v2 - vec.v2, v3 - vec.v3);
	};

	TVec3 operator* (const TVec3& vec) const
	{
		return TVec3(v1 * vec.v1, v2 * vec.v2, v3 * vec.v3);
	};

	TVec3 operator/ (const TVec3& vec) const
	{
		return TVec3(v1 / vec.v1, v2 / vec.v2, v3 / vec.v3);
	};


	TVec3 operator+ (T v) const
	{
		return TVec3(v1 + v, v2 + v, v3 + v);
	};

	TVec3 operator- (T v) const
	{
		return TVec3(v1 - v, v2 - v, v3 - v);
	};

	TVec3 operator* (T v) const
	{
		return TVec3(v1 * v, v2 * v, v3 * v);
	};

	TVec3 operator/ (T v) const
	{
		return TVec3(v1 / v, v2 / v, v3 / v);
	};


	TVec3& operator+= (const TVec3& vec)
	{
		v1 += vec.v1;
		v2 += vec.v2;
		v3 += vec.v3;
		return *this;
	};

	TVec3& operator-= (const TVec3& vec)
	{
		v1 -= vec.v1;
		v2 -= vec.v2;
		v3 -= vec.v3;
		return *this;
	};

	TVec3& operator*= (const TVec3& vec)
	{
		v1 *= vec.v1;
		v2 *= vec.v2;
		v3 *= vec.v3;
		return *this;
	};

	TVec3& operator/= (const TVec3& vec)
	{
		v1 /= vec.v1;
		v2 /= vec.v2;
		v3 /= vec.v3;
		return *this;
	};


	TVec3& operator+= (T v)
	{
		v1 += v;
		v2 += v;
		v3 += v;
		return *this;
	};

	TVec3& operator-= (T v)
	{
		v1 -= v;
		v2 -= v;
		v3 -= v;
		return *this;
	};

	TVec3& operator*= (T v)
	{
		v1 *= v;
		v2 *= v;
		v3 *= v;
		return *this;
	};

	TVec3& operator/= (T v)
	{
		v1 /= v;
		v2 /= v;
		v3 /= v;
		return *this;
	};

};




template <class T>
class TVec4
{
public:
	union
	{
		T _v[4];
		struct
		{
			T v1, v2, v3, v4;
		};
		struct
		{
			T x, y, z, w;
		};
	};

	TVec4()
	{};

	TVec4(T _v1, T _v2, T _v3, T _v4)
		:v1(_v1), v2(_v2), v3(_v3), v4(_v4)
	{};

	TVec4(const TVec4& v)
		: v1(v.v1), v2(v.v2), v3(v.v3), v4(v.v4)
	{};


	TVec4 operator- () const
	{
		return TVec4(-v1, -v2, -v3, -v4);
	};

	TVec4 operator+ (const TVec4& vec) const
	{
		return TVec4(v1 + vec.v1, v2 + vec.v2, v3 + vec.v3, v4 + vec.v4);
	};

	TVec4 operator- (const TVec4& vec) const
	{
		return TVec4(v1 - vec.v1, v2 - vec.v2, v3 - vec.v3, v4 - vec.v4);
	};

	TVec4 operator* (const TVec4& vec) const
	{
		return TVec4(v1 * vec.v1, v2 * vec.v2, v3 * vec.v3, v4 * vec.v4);
	};

	TVec4 operator/ (const TVec4& vec) const
	{
		return TVec4(v1 / vec.v1, v2 / vec.v2, v3 / vec.v3, v4 / vec.v4);
	};


	TVec4 operator+ (T v) const
	{
		return TVec4(v1 + v, v2 + v, v3 + v, v4 + v);
	};

	TVec4 operator- (T v) const
	{
		return TVec4(v1 - v, v2 - v, v3 - v, v4 - v);
	};

	TVec4 operator* (T v) const
	{
		return TVec4(v1 * v, v2 * v, v3 * v, v4 * v);
	};

	TVec4 operator/ (T v) const
	{
		return TVec4(v1 / v, v2 / v, v3 / v, v4 / v);
	};


	TVec4& operator+= (const TVec4& vec)
	{
		v1 += vec.v1;
		v2 += vec.v2;
		v3 += vec.v3;
		v4 += vec.v4;
		return *this;
	};

	TVec4& operator-= (const TVec4& vec)
	{
		v1 -= vec.v1;
		v2 -= vec.v2;
		v3 -= vec.v3;
		v4 -= vec.v4;
		return *this;
	};

	TVec4& operator*= (const TVec4& vec)
	{
		v1 *= vec.v1;
		v2 *= vec.v2;
		v3 *= vec.v3;
		v4 *= vec.v4;
		return *this;
	};

	TVec4& operator/= (const TVec4& vec)
	{
		v1 /= vec.v1;
		v2 /= vec.v2;
		v3 /= vec.v3;
		v4 /= vec.v4;
		return *this;
	};


	TVec4& operator+= (T v)
	{
		v1 += v;
		v2 += v;
		v3 += v;
		v4 += v;
		return *this;
	};

	TVec4& operator-= (T v)
	{
		v1 -= v;
		v2 -= v;
		v3 -= v;
		v4 -= v;
		return *this;
	};

	TVec4& operator*= (T v)
	{
		v1 *= v;
		v2 *= v;
		v3 *= v;
		v4 *= v;
		return *this;
	};

	TVec4& operator/= (T v)
	{
		v1 /= v;
		v2 /= v;
		v3 /= v;
		v4 /= v;
		return *this;
	};

};

