#include "utils\matrix.h"


Mx2d::Mx2d()
	: m11(0), m12(0), m21(0), m22(0)
{};


Mx2d::Mx2d(float _m11, float _m12, float _m21, float _m22)
	: m11(_m11), m12(_m12), m21(_m21), m22(_m22)
{};


Mx2d::Mx2d(const Mx2d& _m)
	: m11(_m.m11), m12(_m.m12), m21(_m.m21), m22(_m.m22)
{};


Vec2f Mx2d::mul(const Vec2f& _v) const
{
	Vec2f out;
	return mul(_v, out);
}


Vec2f Mx2d::operator * (const Vec2f& _v) const
{
	Vec2f out;
	return mul(_v, out);
}


Mx2d Mx2d::mul(const Mx2d& _m) const
{
	Mx2d out;
	return mul(_m, out);
}


Mx2d Mx2d::operator * (Mx2d _m) const
{
	Mx2d out;
	return mul(_m, out);
}



Mx3d::Mx3d() : m11(0), m12(0), m13(0), m21(0), m22(0), m23(0), m31(0), m32(0), m33(0)
{};


Mx3d::Mx3d(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33)
	: m11(_m11), m12(_m12), m13(_m13), m21(_m21), m22(_m22), m23(_m23), m31(_m31), m32(_m32), m33(_m33)
{};


Mx3d::Mx3d(const Mx3d& _m)
	: m11(_m.m11), m12(_m.m12), m13(_m.m13), m21(_m.m21), m22(_m.m22), m23(_m.m23), m31(_m.m31), m32(_m.m32), m33(_m.m33)
{};


Vec3f Mx3d::mul(const Vec3f& _v) const
{
	Vec3f out;
	return mul(_v, out);
}

Vec3f Mx3d::operator * (const Vec3f& _v) const
{
	Vec3f out;
	return mul(_v, out);
}


Vec2f Mx3d::mul(const Vec2f& _v) const
{
	Vec2f out;
	return mul(_v, out);
}

Vec2f Mx3d::operator * (const Vec2f& _v) const
{
	Vec2f out;
	return mul(_v, out);
}


Mx3d Mx3d::mul(const Mx3d& _m) const
{
	Mx3d out;
	return mul(_m, out);
}

Mx3d Mx3d::operator * (const Mx3d& _m) const
{
	Mx3d out;
	return mul(_m, out);
}



Mx2d& identity(Mx2d& _m)
{
	_m.m11 = 1.f;
	_m.m12 = 0.f;

	_m.m21 = 0.f;
	_m.m22 = 1.f;

	return _m;
};


Mx3d& identity(Mx3d& _m)
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


Mx3d& mxTranslate(Mx3d& _m, float _translateX, float _translateY)
{
	_m.m11 = 1.f;
	_m.m12 = 0.f;
	_m.m13 = _translateX;

	_m.m21 = 0.f;
	_m.m22 = 1.f;
	_m.m23 = _translateY;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
}


Mx3d& mxRotate(Mx3d& _m, float _rotationAngle)
{
	float cosO = std::cos(_rotationAngle);
	float sinO = std::sin(_rotationAngle);

	_m.m11 = cosO;
	_m.m12 = -sinO;
	_m.m13 = 0.f;

	_m.m21 = sinO;
	_m.m22 = cosO;
	_m.m23 = 0.f;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
};


Mx3d& mxRotate(Mx3d& _m, float _rotationAngle, const Vec2f& _rotationPoint)
{
	//	cosO	-sinO		cosO * -Tx + -sinO * -Ty + Tx
	//	sinO	cosO		sinO * -Tx + cosO * -Ty + Ty
	//	0		0			1

	float cosO = std::cos(_rotationAngle);
	float sinO = std::sin(_rotationAngle);

	_m.m11 = cosO;
	_m.m12 = -sinO;
	_m.m13 = cosO * -_rotationPoint.x + -sinO * -_rotationPoint.y + _rotationPoint.x;

	_m.m21 = sinO;
	_m.m22 = cosO;
	_m.m23 = sinO * -_rotationPoint.x + cosO * -_rotationPoint.y + _rotationPoint.y;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
};


Mx3d& mxScale(Mx3d& _m, float _scaleX, float _scaleY)
{
	_m.m11 = _scaleX;
	_m.m12 = 0.f;
	_m.m13 = 0.f;

	_m.m21 = 0.f;
	_m.m22 = _scaleY;
	_m.m23 = 0.f;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
};


Mx3d& mxScaleRotateTranslate(Mx3d& _m, float _scaleX, float _scaleY, float _rotationAngle, float _translateX, float _translateY)
{
	//	cosO * Sx		-sinO * Sy		Tx
	//	sinO * Sx		cosO * Sy		Ty
	//	0				0				1

	float cosO = std::cos(_rotationAngle);
	float sinO = std::sin(_rotationAngle);

	_m.m11 = cosO * _scaleX;
	_m.m12 = -sinO * _scaleY;
	_m.m13 = _translateX;


	_m.m21 = sinO * _scaleX;
	_m.m22 = cosO * _scaleY;
	_m.m23 = _translateY;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
};


Mx3d& mxScaleTranslate(Mx3d& _m, float _scaleX, float _scaleY, float _translateX, float _translateY)
{
	//	1 * Sx		0 * Sy		Tx
	//	0 * Sx		1 * Sy		Ty
	//	0				0				1

	_m.m11 = _scaleX;
	_m.m12 = 0.f;
	_m.m13 = _translateX;


	_m.m21 = 0.f;
	_m.m22 = _scaleY;
	_m.m23 = _translateY;

	_m.m31 = 0.f;
	_m.m32 = 0.f;
	_m.m33 = 1.f;

	return _m;
};

