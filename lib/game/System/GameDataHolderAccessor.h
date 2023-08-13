#pragma once

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
class ISceneObj;
class LiveActor;
}  // namespace al

class GameDataHolderAccessor {  // maybe extends GameDataHolderWriter?
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);

private:
    al::ISceneObj* mSceneObj;
};

namespace rs {
bool isInvalidChangeStage(const al::LiveActor*);
}
