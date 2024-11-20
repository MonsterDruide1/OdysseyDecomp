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

class GameDataHolderAccessor : public GameDataHolderWriter {
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);
};
