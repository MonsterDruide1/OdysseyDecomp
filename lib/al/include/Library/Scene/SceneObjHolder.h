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

private:
    ISceneObj* (*mCreator)(s32);
    ISceneObj** mSceneObjArray;
    s32 mArraySize;
};

}  // namespace al
