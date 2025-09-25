#pragma once

#include <basis/seadTypes.h>

namespace al {
class ISceneObj;
struct ActorInitInfo;

class SceneObjHolder {
public:
    SceneObjHolder(ISceneObj* (*creator)(s32), s32 size);
    ISceneObj* create(s32 index);
    ISceneObj* tryGetObj(s32 index) const;
    ISceneObj* getObj(s32 index) const;
    bool isExist(s32 index) const;
    void setSceneObj(ISceneObj* obj, s32 index);
    void initAfterPlacementSceneObj(const ActorInitInfo& info);

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
