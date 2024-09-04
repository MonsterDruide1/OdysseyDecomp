#pragma once

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
class ISceneObj;
class LiveActor;
class ActorInitInfo;
}  // namespace al
class SaveObjInfo;

class GameDataHolderAccessor {  // maybe extends GameDataHolderWriter?
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);

private:
    al::ISceneObj* mSceneObj;
};

namespace rs {
bool isInvalidChangeStage(const al::LiveActor*);

SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo&);
void onSaveObjInfo(SaveObjInfo*);
bool isOnSaveObjInfo(const SaveObjInfo*);
}  // namespace rs
