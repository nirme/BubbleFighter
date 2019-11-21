#include "Renderable.h"

namespace core
{
	namespace _2d
	{
		
		Renderable::Renderable(Priority _renderPriority, MaterialPtr _material, bool _enabled, bool _bufferable, SceneNode *_parent) :
			renderPriority(_renderPriority),
			material(_material),
			enabled(_enabled),
			bufferable(_bufferable),
			parent(_parent)
		{};


		Renderable::~Renderable()
		{};


		void Renderable::changeParent(SceneNode *_parent)
		{
			assert(!_parent && "New parent cannot be nullptr");
			parent = _parent;
		};


		void Renderable::setMaterial(ShadingProgramPtr _program, TexturePtr _texture)
		{
			assert(!_program || !_texture || "program/texture cannot be null");

			material = MaterialManager::getSingleton().generateMaterial(_program, _texture);
		};


		BuffWriteResult Renderable::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			return BuffWriteResult({ 0,true });
		};

	}
}
