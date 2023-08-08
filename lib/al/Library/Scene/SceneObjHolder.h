#pragma once

namespace al {
class ISceneObj;
class ActorInitInfo;

class SceneObjHolder {
public:
    SceneObjHolder(ISceneObj* (*)(int), int);
    ISceneObj* create(int);
    ISceneObj* tryGetObj(int) const;
    ISceneObj* getObj(int) const;
    bool isExist(int) const;
    void setSceneObj(ISceneObj*, int);
    void initAfterPlacementSceneObj(const ActorInitInfo&);

private:
    ISceneObj* (*mCreator)(int);
    ISceneObj** mSceneObjArray;
    int mArraySize;
};

class IUseSceneObjHolder {
public:
    virtual al::SceneObjHolder* getSceneObjHolder() const = 0;

    static const char* sSceneObjName;
};

}  // namespace al
