#include "SceneNode.h"


namespace core
{
    namespace _2d
    {

        AxisAlignedBox SceneNode::_boundingBoxImpl() const
        {
            return AxisAlignedBox();
        };

        void SceneNode::updateBoundingBox() const
        {
            assert(!boundingBoxNeedUpdate && "Cashed bounding box don't require updates");

            AxisAlignedBox boundingBox = _boundingBoxImpl();

            if (children.size())
            {
                for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
                    boundingBox.merge((*it)->getBoundingBox());
            }
            aaBox = boundingBox;

            boundingBoxNeedUpdate = true;
        };

        void SceneNode::_updateWorldTransform() const
        {
            assert(!cashedTransformNeedUpdate && "Cashed matrix don't require updates");

            cashedWorldTransform = parent->getWorldTransform() * affine2DMatrix(scale, rotation, position);
            cashedTransformNeedUpdate = false;
        };

        void SceneNode::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
        {};



		SceneNode::SceneNode(const char *_name) :
			name(_name ? _name : ""),
			parent(nullptr),
			scale(1.0f),
			rotation(0.0f),
			position(0.0f),
			cashedTransformNeedUpdate(true),
			boundingBoxNeedUpdate(true)
		{};



        SceneNode::~SceneNode()
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                delete (*it);
                (*it) = nullptr;
            }
        };


		void SceneNode::setParent(SceneNode *_parent)
		{
			parent = _parent;
		};

		SceneNode *SceneNode::getParent() const
		{
			return parent;
		};


		void SceneNode::setName(const std::string &_name)
		{
			name = _name;
		};

		const std::string &SceneNode::getName() const
		{
			return name;
		};


		void SceneNode::setScale(const Vector2 &_scale)
		{
			scale = _scale;
		};

		const Vector2 &SceneNode::getScale() const
		{
			return scale;
		};

		void SceneNode::serRotation(const Quaternion &_rotation)
		{
			rotation = _rotation;
		};

		const Quaternion &SceneNode::getRotation() const
		{
			return rotation;
		};

		void SceneNode::setPosition(const Vector2 &_position)
		{
			position = _position;
		};

		const Vector2 &SceneNode::getPosition() const
		{
			return position;
		};


		SceneNode *SceneNode::getNodeByName(const std::string &_name)
		{
			if (name.compare(_name) == 0)
				return this;

			SceneNode *node;
			for (unsigned int i = 0, iEnd = children.size(); i < iEnd; ++i)
			{
				if (node = children[i]->getNodeByName(_name))
					return node;
			}

			return nullptr;
		};


		SceneNode::ChildNodeIterator SceneNode::getChildIterator(bool _end)
        {
            return (!_end) ? children.begin() : children.end();
        };

        void SceneNode::appendChild(SceneNode* _child)
        {
            SceneNode *prevParent = _child->getParent();
            if (prevParent)
                prevParent->removeChild(_child);

            children.push_back(_child);
        };

        void SceneNode::removeChild(SceneNode* _child)
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                if (_child == (*it))
                {
                    children.erase(it);
                    _child->setParent(nullptr);
                    return;
                }
            }
        };

        const AxisAlignedBox& SceneNode::getBoundingBox() const
        {
            if (!boundingBoxNeedUpdate)
                updateBoundingBox();

            return aaBox;
        };

        const Matrix3& SceneNode::getWorldTransform() const
        {
            if (cashedTransformNeedUpdate)
                _updateWorldTransform();

            return cashedWorldTransform;
        };


        void SceneNode::findVisibleRenderables(Camera *_camera, RenderQueue *_queue, const AxisAlignedBox *_bounds) const
        {
            if (_bounds->isOverlapping(getBoundingBox()))
            {
                _findVisibleRenderablesImpl(_camera, _queue, _bounds);

                for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
                    (*it)->findVisibleRenderables(_camera, _queue, _bounds);
            }

        };


    };

}
