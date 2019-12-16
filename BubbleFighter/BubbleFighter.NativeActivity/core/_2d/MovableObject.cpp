#include "MovableObject.h"



namespace core
{
	namespace _2d
	{

		MovableObject::MovableObject(const std::string &_name) :
			name(_name),
			parent(nullptr),
			scale(1.0f),
			rotation(0.0f),
			position(0.0f),
			boundingBoxNeedUpdate(true)
		{};


		MovableObject::~MovableObject()
		{
			if (parent)
				parent->removeObject(this);
		};


		void MovableObject::setName(const std::string &_name)
		{
			name = _name;
		};


		const std::string &MovableObject::getName() const
		{
			return name;
		};


		void MovableObject::setEnabled(bool _enabled)
		{
			enabled = _enabled;
		};

		bool MovableObject::isEnabled() const
		{
			return enabled;
		};


		void MovableObject::setParent(SceneNode *_parent)
		{
			parent = _parent;

			if (parent)
				parent->invalidateBoundingBox();
		};


		SceneNode *MovableObject::getParent()
		{
			return parent;
		};


		void MovableObject::setScale(Vector2 _scale)
		{
			scale = _scale;
			if (parent)
				parent->invalidateBoundingBox();
		};

		Vector2 MovableObject::getScale() const
		{
			return scale;
		};


		void MovableObject::setRotation(Quaternion _rotation)
		{
			rotation = _rotation;
			if (parent)
				parent->invalidateBoundingBox();
		};

		Quaternion MovableObject::getRotation() const
		{
			return rotation;
		};


		void MovableObject::setPosition(Vector2 _position)
		{
			position = _position;
			if (parent)
				parent->invalidateBoundingBox();
		};

		Vector2 MovableObject::getPosition() const
		{
			return position;
		};


		void MovableObject::invalidateWorldTransform()
		{
			boundingBoxNeedUpdate = true;
			cashedTransformNeedUpdate = true;
			_invalidateWorldTransformImpl();
		};


		void MovableObject::findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
		{
			if (enabled && getBoundingBox().isOverlapping(*_bounds))
				_findVisibleRenderablesImpl(_camera, _queue, _bounds);
		};

		const AxisAlignedBox &MovableObject::getBoundingBox() const
		{
			if (boundingBoxNeedUpdate)
			{
				cashedBoundingBox = _boundingBoxImpl();
				boundingBoxNeedUpdate = false;
			}

			return cashedBoundingBox;
		};


		const Matrix3 &MovableObject::getWorldTransform() const
		{
			if (cashedTransformNeedUpdate)
			{
				cashedWorldTransform = parent->getWorldTransform() * affine2DMatrix(scale, rotation, position);
				cashedTransformNeedUpdate = false;
			}

			return cashedWorldTransform;
		};

	}
}