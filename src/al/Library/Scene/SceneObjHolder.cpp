#include "al/Library/Scene/SceneObjHolder.h"

#include "al/Library/Scene/ISceneObj.h"

namespace al {

SceneObjHolder::SceneObjHolder(al::ISceneObj* (*creator)(int), int size)
    : mCreator(creator), mArraySize(size) {
    mSceneObjArray = new ISceneObj*[size];

    for (int i = 0; i < mArraySize; i++) {
        mSceneObjArray[i] = nullptr;
    }
}

ISceneObj* SceneObjHolder::create(int index) {
    if (mSceneObjArray[index])  // already exists
        return mSceneObjArray[index];

    mSceneObjArray[index] = mCreator(index);
    mSceneObjArray[index]->initSceneObj();
    return mSceneObjArray[index];
}

ISceneObj* SceneObjHolder::tryGetObj(int index) const {
    return mSceneObjArray[index];
}
ISceneObj* SceneObjHolder::getObj(int index) const {
    return mSceneObjArray[index];
}
bool SceneObjHolder::isExist(int index) const {
    return mSceneObjArray[index] != nullptr;
}

void SceneObjHolder::setSceneObj(al::ISceneObj* obj, int index) {
    mSceneObjArray[index] = obj;
}

void SceneObjHolder::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    for (int i = 0; i < mArraySize; i++) {
        if (mSceneObjArray[i])
            mSceneObjArray[i]->initAfterPlacementSceneObj(info);
    }
}

}  // namespace al
