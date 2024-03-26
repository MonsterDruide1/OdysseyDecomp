#pragma once

#include "System/GameDataHolderWriter.h"

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
class ISceneObj;
class LiveActor;
}  // namespace al

class GameDataHolderAccessor : public GameDataHolderWriter {  // maybe extends GameDataHolderWriter?
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);
};

namespace rs {
bool isInvalidChangeStage(const al::LiveActor*);
}
