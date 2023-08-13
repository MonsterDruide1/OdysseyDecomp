#pragma once

#include <math/seadVector.h>

class PlayerActorHakoniwa;

namespace al {
class LiveActor;
class Scene;
class SensorMsg;
class IUseSceneObjHolder;
}  // namespace al

// from Starlight's header files. TODO clean this up, and include them in the proper places
namespace rs {
uint32_t getStageShineAnimFrame(const al::LiveActor*, const char*);

PlayerActorHakoniwa* getPlayerActor(const al::Scene*);

bool isInChangeStageArea(al::LiveActor const*, sead::Vector3f const*);

bool isInvalidChangeStage(al::LiveActor const*);

bool isMsgCapTouchWall(al::SensorMsg const*);

void buyCap(al::IUseSceneObjHolder const*, char const*);

int getActiveQuestNum(al::IUseSceneObjHolder const*);
int getActiveQuestNo(al::IUseSceneObjHolder const*);
const char* getActiveQuestLabel(al::IUseSceneObjHolder const*);
void requestShowHtmlViewer(al::IUseSceneObjHolder const*);
}  // namespace rs
