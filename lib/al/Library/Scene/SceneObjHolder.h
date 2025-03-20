#pragma once

#include <basis/seadTypes.h>

namespace al {
class ISceneObj;
class ActorInitInfo;

class SceneObjHolder {
public:
    SceneObjHolder(ISceneObj* (*)(s32), s32);
    ISceneObj* create(s32);
    ISceneObj* tryGetObj(s32) const;
    ISceneObj* getObj(s32) const;
    bool isExist(s32) const;
    void setSceneObj(ISceneObj*, s32);
    void initAfterPlacementSceneObj(const ActorInitInfo&);

    template <typename T>
    inline T* tryGetObj() const {
        return static_cast<T*>(tryGetObj(T::sSceneObjId));
    }

    template <typename T>
    inline T* getObj() const {
        return static_cast<T*>(getObj(T::sSceneObjId));
    }

private:
    ISceneObj* (*mCreator)(s32);
    ISceneObj** mSceneObjArray;
    s32 mArraySize;
};

}  // namespace al
