#pragma once

namespace al
{
    class SceneObjHolder;

    class IUseSceneObjHolder
    {
    public:
        virtual al::SceneObjHolder* getSceneObjHolder() const = 0;
    };
};