#pragma once

#include "Material.h"


void Graphic::Material::apply(VideoDriver* vDriver) const
{
	vDriver->_setProgram(program->getId);

	for (int i = 0; i < texturesWithUnitIndex.size(); ++i)
	{
		vDriver->_setTexture2D(texturesWithUnitIndex[i].first, texturesWithUnitIndex[i].second->getId());
	}
};

