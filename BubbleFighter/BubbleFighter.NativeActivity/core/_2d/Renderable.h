#pragma once

#include <memory>
#include "../Matrix3.h"
#include "SceneNode.h"
#include "../Material.h"


namespace core
{
	namespace _2d
	{
		class VertexData;
		typedef unsigned char Priority;

		class Renderable
		{
		protected:

			SceneNode *parent;
			Priority renderPriority;
			MaterialId materialId;

			MaterialPtr material;

			// renderable can have own vertex buff (particle systems etc..)
			bool haveVertexDataBuffer;


			virtual void _loadResImpl() = 0;

		public:

			Renderable(Priority _renderPriority, MaterialPtr _material, SceneNode *_parent = nullptr, bool _haveVertexDataBuffer = false) :
				parent(_parent), 
				renderPriority(_renderPriority), 
				materialId(_material->getHandle()), 
				material(_material), 
				haveVertexDataBuffer(_haveVertexDataBuffer)
			{};

			virtual ~Renderable()
			{};


			void changeParent(SceneNode *_parent)
			{
				assert(!_parent && "New parent cannot be nullptr");
				parent = _parent;
			};


			MaterialPtr getMaterial;

			const Matrix3& getTransform()
			{
				if (parent)
					return parent->getWorldTransform();
				return Matrix3::IDENTITY;
			};

			inline Priority getPriority() const
			{
				return renderPriority;
			};

			inline MaterialId getMaterialId() const
			{
				return materialId;
			};

			inline bool ownVertexDataBuffer() { return haveVertexDataBuffer; }

			virtual const VertexData* getSpriteVertexData( void *_buffer, unsigned int _bufferSize, unsigned int &_bytesWritten) { return nullptr; };

		};

	}
}
