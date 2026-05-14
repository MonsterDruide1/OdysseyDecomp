#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class IUseSceneObjHolder;
class GamePadSystem;
class LayoutActor;
class LiveActor;
class Scene;
}  // namespace al
class GameDataHolder;

class GamePadPlayStyleInfo {
public:
    s32 mControllerCount = 0;
    s32 mPort0PlayStyle = 0;
    s32 mPort1PlayStyle = 0;
};

static_assert(sizeof(GamePadPlayStyleInfo) == 0xc);

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder* user);
void changeSeparatePlayMode(al::Scene* scene, bool isSeparatePlay);
void setSeparatePlayMode(al::Scene* scene, bool isSeparatePlay);
bool isTriggerUiDecide(const al::IUseSceneObjHolder* user);
bool isTriggerUiDecide(const GameDataHolder* gameDataHolder);
bool isTriggerUiCancel(const al::IUseSceneObjHolder* user);
bool isTriggerUiSelect(const al::IUseSceneObjHolder* user);
bool isTriggerUiPause(const al::IUseSceneObjHolder* user);
bool isTriggerUiLeft(const al::IUseSceneObjHolder* user);
bool isTriggerUiRight(const al::IUseSceneObjHolder* user);
bool isTriggerUiUp(const al::IUseSceneObjHolder* user);
bool isTriggerUiDown(const al::IUseSceneObjHolder* user);
bool isTriggerUiR(const al::IUseSceneObjHolder* user);
bool isTriggerUiL(const al::IUseSceneObjHolder* user);
bool isTriggerUiZR(const al::IUseSceneObjHolder* user);
bool isTriggerUiZL(const al::IUseSceneObjHolder* user);
bool isTriggerUiX(const al::IUseSceneObjHolder* user);
bool isTriggerUiY(const al::IUseSceneObjHolder* user);
bool isTriggerUiAnyABXY(const al::IUseSceneObjHolder* user);
bool isRepeatUiUp(const al::IUseSceneObjHolder* user);
bool isRepeatUiDown(const al::IUseSceneObjHolder* user);
bool isRepeatUiRight(const al::IUseSceneObjHolder* user);
bool isRepeatUiLeft(const al::IUseSceneObjHolder* user);
bool isHoldUiDecide(const al::IUseSceneObjHolder* user);
bool isHoldUiCancel(const al::IUseSceneObjHolder* user);
bool isHoldUiSelect(const al::IUseSceneObjHolder* user);
bool isHoldUiL(const al::IUseSceneObjHolder* user);
bool isHoldUiR(const al::IUseSceneObjHolder* user);
bool isHoldUiY(const al::IUseSceneObjHolder* user);
bool isHoldUiUp(const al::IUseSceneObjHolder* user);
bool isHoldUiDown(const al::IUseSceneObjHolder* user);
bool isHoldUiLeft(const al::IUseSceneObjHolder* user);
bool isHoldUiRight(const al::IUseSceneObjHolder* user);
bool isHoldSeparatePlayStart1P(const al::IUseSceneObjHolder* user);
bool isHoldSeparatePlayStart2P(const al::IUseSceneObjHolder* user);
bool isTriggerMapOpen(const al::IUseSceneObjHolder* user);
bool isHoldSeparatePlayEnd(const al::IUseSceneObjHolder* user);
sead::Vector2f getUiLeftStick(const al::IUseSceneObjHolder* user);
sead::Vector2f getUiRightStick(const al::IUseSceneObjHolder* user);
bool isTriggerMapClose(const al::IUseSceneObjHolder* user);
bool isTriggerCollectionListOpenInMap(const al::IUseSceneObjHolder* user);
bool isTriggerCollectionListCloseInMap(const al::IUseSceneObjHolder* user);
bool isTriggerAppearSkipGuide(const al::IUseSceneObjHolder* user);
bool isTriggerUiRacePause(const al::IUseSceneObjHolder* user);
bool isTriggerSnapShotMode(const al::IUseSceneObjHolder* user);
bool isTriggerAmiiboMode(const al::IUseSceneObjHolder* user);
bool isHoldAmiiboMode(const al::IUseSceneObjHolder* user);
bool isTriggerToggleSnapShotModeLayout(const al::IUseSceneObjHolder* user);
bool isTriggerAppearSnapShotLayout(const al::IUseSceneObjHolder* user);
bool isTriggerIncrementPostProcessingFilterPreset(const al::IUseSceneObjHolder* user);
bool isTriggerDecrementPostProcessingFilterPreset(const al::IUseSceneObjHolder* user);
void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LiveActor* actor);
void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LayoutActor* layoutActor);
bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LiveActor* actor);
bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LayoutActor* layoutActor);
}  // namespace rs

namespace ControllerAppletFunction {
bool tryReconnectGamePad(al::GamePadSystem* gamePadSystem);
bool connectControllerSinglePlay(al::GamePadSystem* gamePadSystem);
bool connectControllerSeparatePlay(al::GamePadSystem* gamePadSystem);
}  // namespace ControllerAppletFunction
