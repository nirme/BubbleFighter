#include "MaterialManager.h"


template<>
core::_2d::MaterialManager* Singleton<core::_2d::MaterialManager>::impl = nullptr;

namespace core
{
    namespace _2d
    {

        MaterialManager::MaterialManager() :
            nextFreeId(1)
        {};


        MaterialPtr MaterialManager::generateMaterial(ShadingProgramPtr _program, TexturePtr _texture)
        {
            auto it = materialIdsList.find({_program->getHandle(), _texture->getHandle()});
            if (it != materialIdsList.end())
                return (*it).second;

            MaterialPtr material(new Material({nextFreeId++, _program, _texture}));
            auto newEntry = materialIdsList.emplace(MaterialKey(_program->getHandle(), _texture->getHandle()), material);

            materialsByIds.emplace(material->id, material);
            return material;
        };

        MaterialPtr MaterialManager::getById(MaterialId _id)
        {
            auto it = materialsByIds.find(_id);
            return it != materialsByIds.end() ? (*it).second : nullptr;
        };

    }
}
