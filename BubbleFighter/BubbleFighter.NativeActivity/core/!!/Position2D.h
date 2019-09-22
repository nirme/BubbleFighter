#pragma once


#include "utils\vector.h"
#include "utils\matrix.h"


class Position2D
{
private:
	Position2D* parent;
	unsigned short parentUpdateCounter;

	Vec2f translation;
	float rotation;
	Vec2f scale;
	bool localSpaceCurrent;

	Mx3d worldSpace;

	unsigned short updateCounter;


protected:

	inline const Mx3d& updateMatrix()
	{
		if (!localSpaceCurrent || !parent && parentUpdateCounter != parent->getUpdateCounter())
		{
			mxScaleRotateTranslate(worldSpace, scale, rotation, translation);

			if (parent)
				worldSpace = worldSpace * parent->getMatrix();

			localSpaceCurrent = true;
			parentUpdateCounter = parent->getUpdateCounter();

			++updateCounter;
		}

		return worldSpace;
	};


public:

	Position2D()
	{};

	inline unsigned short getUpdateCounter() const	{	return updateCounter;	};
	inline const Mx3d& getMatrix()					{	return updateMatrix();	};

	void setParent(Position2D* _parent = nullptr);

	void applyTranslation(const Vec2f& _translation, bool _override = true);
	void applyRotation(float _rotation, bool _override = true);
	void applyScaling(const Vec2f& _scale, bool _override = true);

};
