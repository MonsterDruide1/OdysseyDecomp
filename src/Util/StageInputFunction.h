#pragma once

#include <math/seadVector.h>

namespace al {
class IUseSceneObjHolder;
class GamePadSystem;
class LayoutActor;
class LiveActor;
class Scene;
}  // namespace al
class GameDataHolder;
class GamePadPlayStyleInfo;

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder*);
void changeSeparatePlayMode(al::Scene*, bool);
void setSeparatePlayMode(al::Scene*, bool);
bool isTriggerUiDecide(const al::IUseSceneObjHolder*);
bool isTriggerUiDecide(const GameDataHolder*);
bool isTriggerUiCancel(const al::IUseSceneObjHolder*);
bool isTriggerUiSelect(const al::IUseSceneObjHolder*);
bool isTriggerUiPause(const al::IUseSceneObjHolder*);
bool isTriggerUiLeft(const al::IUseSceneObjHolder*);
bool isTriggerUiRight(const al::IUseSceneObjHolder*);
bool isTriggerUiUp(const al::IUseSceneObjHolder*);
bool isTriggerUiDown(const al::IUseSceneObjHolder*);
bool isTriggerUiR(const al::IUseSceneObjHolder*);
bool isTriggerUiL(const al::IUseSceneObjHolder*);
bool isTriggerUiZR(const al::IUseSceneObjHolder*);
bool isTriggerUiZL(const al::IUseSceneObjHolder*);
bool isTriggerUiX(const al::IUseSceneObjHolder*);
bool isTriggerUiY(const al::IUseSceneObjHolder*);
bool isTriggerUiAnyABXY(const al::IUseSceneObjHolder*);
bool isRepeatUiUp(const al::IUseSceneObjHolder*);
bool isRepeatUiDown(const al::IUseSceneObjHolder*);
bool isRepeatUiRight(const al::IUseSceneObjHolder*);
bool isRepeatUiLeft(const al::IUseSceneObjHolder*);
bool isHoldUiDecide(const al::IUseSceneObjHolder*);
bool isHoldUiCancel(const al::IUseSceneObjHolder*);
bool isHoldUiSelect(const al::IUseSceneObjHolder*);
bool isHoldUiL(const al::IUseSceneObjHolder*);
bool isHoldUiR(const al::IUseSceneObjHolder*);
bool isHoldUiY(const al::IUseSceneObjHolder*);
bool isHoldUiUp(const al::IUseSceneObjHolder*);
bool isHoldUiDown(const al::IUseSceneObjHolder*);
bool isHoldUiLeft(const al::IUseSceneObjHolder*);
bool isHoldUiRight(const al::IUseSceneObjHolder*);
bool isHoldSeparatePlayStart1P(const al::IUseSceneObjHolder*);
bool isHoldSeparatePlayStart2P(const al::IUseSceneObjHolder*);
bool isTriggerMapOpen(const al::IUseSceneObjHolder*);
bool isHoldSeparatePlayEnd(const al::IUseSceneObjHolder*);
const sead::Vector2f& getUiLeftStick(const al::IUseSceneObjHolder*);
const sead::Vector2f& getUiRightStick(const al::IUseSceneObjHolder*);
bool isTriggerMapClose(const al::IUseSceneObjHolder*);
bool isTriggerCollectionListOpenInMap(const al::IUseSceneObjHolder*);
bool isTriggerCollectionListCloseInMap(const al::IUseSceneObjHolder*);
bool isTriggerAppearSkipGuide(const al::IUseSceneObjHolder*);
bool isTriggerUiRacePause(const al::IUseSceneObjHolder*);
bool isTriggerSnapShotMode(const al::IUseSceneObjHolder*);
bool isTriggerAmiiboMode(const al::IUseSceneObjHolder*);
bool isHoldAmiiboMode(const al::IUseSceneObjHolder*);
bool isTriggerToggleSnapShotModeLayout(const al::IUseSceneObjHolder*);
bool isTriggerAppearSnapShotLayout(const al::IUseSceneObjHolder*);
bool isTriggerIncrementPostProcessingFilterPreset(const al::IUseSceneObjHolder*);
bool isTriggerDecrementPostProcessingFilterPreset(const al::IUseSceneObjHolder*);
void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo*, const al::LiveActor*);
void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo*, const al::LayoutActor*);
bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo*, const al::LiveActor*);
bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo*, const al::LayoutActor*);
}  // namespace rs

namespace ControllerAppletFunction {
bool tryReconnectGamePad(al::GamePadSystem*);
bool connectControllerSinglePlay(al::GamePadSystem*);
bool connectControllerSeparatePlay(al::GamePadSystem*);
}  // namespace ControllerAppletFunction
