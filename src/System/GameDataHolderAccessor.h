#pragma once

#include "System/GameDataHolderWriter.h"

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
class ISceneObj;
class LiveActor;
class ActorInitInfo;
}  // namespace al
class SaveObjInfo;

class GameDataHolderAccessor : public GameDataHolderWriter {  // maybe extends GameDataHolderWriter?
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);
};

namespace rs {
bool isInvalidChangeStage(const al::LiveActor* actor);
bool isKidsMode(const al::LiveActor* actor);

SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo&);
void onSaveObjInfo(SaveObjInfo*);
bool isOnSaveObjInfo(const SaveObjInfo*);
}  // namespace rs
