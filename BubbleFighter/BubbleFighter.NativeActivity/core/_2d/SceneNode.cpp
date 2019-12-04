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

        void SceneNode::_findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const
        {};


        SceneNode::~SceneNode()
        {
            for (auto it = children.begin(); it != children.end(); ++it)
            {
                delete (*it);
                (*it) = nullptr;
            }
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


        void SceneNode::findVisibleRenderables(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const
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
