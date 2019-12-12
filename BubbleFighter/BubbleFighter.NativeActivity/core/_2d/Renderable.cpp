#include "Renderable.h"

namespace core
{
	namespace _2d
	{
		
		void Renderable::setMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1, TexturePtr _tex2, TexturePtr _tex3, TexturePtr _tex4, TexturePtr _tex5, TexturePtr _tex6, TexturePtr _tex7)
		{
			assert(!_program || !_tex0 || "program/texture cannot be null");

			material = MaterialManager::getSingleton().generateMaterial(_program, _tex0, _tex1, _tex2, _tex3, _tex4, _tex5, _tex6, _tex7);
		};


		Renderable::Renderable(Priority _renderPriority, MaterialPtr _material, bool _enabled, bool _bufferable, Entity *_parent) :
			renderPriority(_renderPriority),
			material(_material),
			enabled(_enabled),
			bufferable(_bufferable),
			parent(_parent)
		{};


		Renderable::~Renderable()
		{};


		void Renderable::changeParent(Entity *_parent)
		{
			assert(!_parent && "New parent cannot be nullptr");
			parent = _parent;
		};


		const Matrix3& Renderable::getTransform() const
		{
			return parent ? parent->getWorldTransform() : Matrix3::IDENTITY;
		};

		Priority Renderable::getPriority() const
		{
			return renderPriority;
		};

		MaterialId Renderable::getMaterialId() const
		{
			return material->id;
		};

		MaterialPtr Renderable::getMaterial() const
		{
			return material;
		};


		void Renderable::setPriority(Priority _renderPriority)
		{
			renderPriority = _renderPriority;
		};


		void Renderable::setEnabled(bool _enabled)
		{
			enabled = _enabled;
		};

		bool Renderable::isEnabled() const
		{
			return enabled;
		};



		BuffWriteResult Renderable::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			return BuffWriteResult({ 0,true });
		};

	}
}
