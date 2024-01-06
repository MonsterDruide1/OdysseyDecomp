#include "Library/Scene/SceneObjHolder.h"

#include "Library/Scene/ISceneObj.h"

namespace al {

SceneObjHolder::SceneObjHolder(ISceneObj* (*creator)(s32), s32 size)
    : mCreator(creator), mArraySize(size) {
    mSceneObjArray = new ISceneObj*[size];

    for (s32 i = 0; i < mArraySize; i++) {
        mSceneObjArray[i] = nullptr;
    }
}

ISceneObj* SceneObjHolder::create(s32 index) {
    if (mSceneObjArray[index])  // already exists
        return mSceneObjArray[index];

    mSceneObjArray[index] = mCreator(index);
    mSceneObjArray[index]->initSceneObj();
    return mSceneObjArray[index];
}

ISceneObj* SceneObjHolder::tryGetObj(s32 index) const {
    return mSceneObjArray[index];
}
ISceneObj* SceneObjHolder::getObj(s32 index) const {
    return mSceneObjArray[index];
}
bool SceneObjHolder::isExist(s32 index) const {
    return mSceneObjArray[index] != nullptr;
}

void SceneObjHolder::setSceneObj(ISceneObj* obj, s32 index) {
    mSceneObjArray[index] = obj;
}

void SceneObjHolder::initAfterPlacementSceneObj(const ActorInitInfo& info) {
    for (s32 i = 0; i < mArraySize; i++) {
        if (mSceneObjArray[i])
            mSceneObjArray[i]->initAfterPlacementSceneObj(info);
    }
}

}  // namespace al
