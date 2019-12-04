#include "ShadingParamsPassthru.h"

namespace core
{

    const Matrix3 &ShadingParamsPassthru::get2dWorldMatrix() const
    {
        return renderable ? renderable->getTransform() : Matrix3::IDENTITY;
    };

    const Matrix3 &ShadingParamsPassthru::get2dViewMatrix() const
    {
        return camera ? camera->getViewMatrix() : Matrix3::IDENTITY;
    };

    Matrix3 ShadingParamsPassthru::get2dWorldViewMatrix() const
    {
        return get2dViewMatrix() * get2dWorldMatrix();
    };

    GLint ShadingParamsPassthru::getTextureId(unsigned int _index) const
    {
        assert(_index < 0 || "Not implemented feature!");
        return material->texture->getId();
    };

    const Vector3 &ShadingParamsPassthru::getAmbientLight() const
    {
        return ambientLight;
    };

    float ShadingParamsPassthru::getTimeElapsed() const
    {
        return timeElapsed;
    };


    void ShadingParamsPassthru::setCurrentRenderable(const _2d::Renderable *_renderable)
    {
        renderable = _renderable;
        material = _2d::MaterialManager::getSingleton().getById(renderable->getMaterialId()).get();
    };

    void ShadingParamsPassthru::setCurrentMaterial(const _2d::Material *_material)
    {
        material = _material;
        renderable = nullptr;
    };

    void ShadingParamsPassthru::setCamera(const _2d::Camera *_camera)
    {
        camera = _camera;
    };

    void ShadingParamsPassthru::setAmbientLightColor(Vector3 _ambientLight)
    {
        ambientLight = _ambientLight;
    };

    void ShadingParamsPassthru::setTimeElapsed(float _timeElapsed)
    {
        timeElapsed = _timeElapsed;
    };
}
