#include "Position2D.h"


Position2D::Position2D() :
	parent(nullptr),
	parentUpdateCounter(-1),
	translation(0.f, 0.f),
	rotation(0.f),
	scale(1.f, 1.f),
	localSpaceCurrent(false),
	worldSpace(),
	updateCounter(0)
{};


void Position2D::setParent(Position2D* _parent)
{
	parent = _parent;
	parentUpdateCounter = parent->getUpdateCounter() - 1;
};


void Position2D::applyTranslation(const Vec2f& _translation, bool _override)
{
	localSpaceCurrent = false;

	if (_override)
		translation = _translation;
	else
		translation += _translation;
};


void Position2D::applyRotation(float _rotation, bool _override)
{
	localSpaceCurrent = false;

	if (_override)
		rotation = _rotation;
	else
		rotation += _rotation;
};


void Position2D::applyScaling(const Vec2f& _scale, bool _override)
{
	localSpaceCurrent = false;

	if (_override)
		scale = _scale;
	else
		scale += _scale;
};
