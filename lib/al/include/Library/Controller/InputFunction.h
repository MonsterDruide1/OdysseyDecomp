#pragma once

#include <controller/seadController.h>
#include <controller/seadControllerMgr.h>
#include <math/seadVector.h>

namespace al {
class IUseCamera;

bool isValidReplayController(u32);
sead::ControllerBase* getReplayController(u32);  // return type might be wrong

bool isPadTypeJoySingle(s32 port);

bool isPadTrigger(s32 port, s32 button);
bool isPadTriggerA(s32 port);
bool isPadTriggerB(s32 port);
bool isPadTriggerX(s32 port);
bool isPadTriggerY(s32 port);
bool isPadTriggerZL(s32 port);
bool isPadTriggerZR(s32 port);
bool isPadTriggerL(s32 port);
bool isPadTriggerR(s32 port);
bool isPadTrigger1(s32 port);
bool isPadTrigger2(s32 port);
bool isPadTriggerUp(s32 port);
bool isPadTriggerDown(s32 port);
bool isPadTriggerLeft(s32 port);
bool isPadTriggerRight(s32 port);
bool isPadTriggerLeftUp(s32 port);
bool isPadTriggerLeftDown(s32 port);
bool isPadTriggerRightUp(s32 port);
bool isPadTriggerRightDown(s32 port);
bool isPadTriggerHome(s32 port);
bool isPadTriggerStart(s32 port);
bool isPadTriggerSelect(s32 port);
bool isPadTriggerPlus(s32 port);
bool isPadTriggerMinus(s32 port);
bool isPadTriggerTouch();
bool isPadTriggerUpLeftStick(s32 port);
bool isPadTriggerDownLeftStick(s32 port);
bool isPadTriggerLeftLeftStick(s32 port);
bool isPadTriggerRightLeftStick(s32 port);
bool isPadTriggerUpRightStick(s32 port);
bool isPadTriggerDownRightStick(s32 port);
bool isPadTriggerLeftRightStick(s32 port);
bool isPadTriggerRightRightStick(s32 port);
bool isPadTriggerAnyABXY(s32 port);
bool isPadTriggerAny(s32 port);
bool isPadTriggerLeftStick(s32 port);
bool isPadTriggerRightStick(s32 port);
bool isPadTriggerPressLeftStick(s32 port);
bool isPadTriggerPressRightStick(s32 port);
bool isPadTriggerUiCursorUp(s32 port);  // TODO implement below
bool isPadTriggerUiCursorDown(s32 port);
bool isPadTriggerUiCursorLeft(s32 port);
bool isPadTriggerUiCursorRight(s32 port);

bool isPadRepeatA(s32 port);
bool isPadRepeatB(s32 port);
bool isPadRepeatX(s32 port);
bool isPadRepeatY(s32 port);
bool isPadRepeatZL(s32 port);
bool isPadRepeatZR(s32 port);
bool isPadRepeatL(s32 port);
bool isPadRepeatR(s32 port);
bool isPadRepeat1(s32 port);
bool isPadRepeat2(s32 port);
bool isPadRepeatUp(s32 port);
bool isPadRepeatDown(s32 port);
bool isPadRepeatLeft(s32 port);
bool isPadRepeatRight(s32 port);
bool isPadRepeatHome(s32 port);
bool isPadRepeatStart(s32 port);
bool isPadRepeatSelect(s32 port);
bool isPadRepeatPlus(s32 port);
bool isPadRepeatMinus(s32 port);
bool isPadRepeatTouch();
bool isPadRepeatUpLeftStick(s32 port);
bool isPadRepeatDownLeftStick(s32 port);
bool isPadRepeatLeftLeftStick(s32 port);
bool isPadRepeatRightLeftStick(s32 port);
bool isPadRepeatUpRightStick(s32 port);
bool isPadRepeatDownRightStick(s32 port);
bool isPadRepeatLeftRightStick(s32 port);
bool isPadRepeatRightRightStick(s32 port);
bool isPadRepeatUiCursorUp(s32 port);  // TODO implement below
bool isPadRepeatUiCursorDown(s32 port);
bool isPadRepeatUiCursorLeft(s32 port);
bool isPadRepeatUiCursorRight(s32 port);

bool isPadHoldPressLeftStick(s32 port);
bool isPadHoldPressRightStick(s32 port);
bool isPadHold(s32, s32);
bool isPadHoldA(s32 port);
bool isPadHoldB(s32 port);
bool isPadHoldX(s32 port);
bool isPadHoldY(s32 port);
bool isPadHoldZL(s32 port);
bool isPadHoldZR(s32 port);
bool isPadHoldL(s32 port);
bool isPadHoldR(s32 port);
bool isPadHold1(s32 port);
bool isPadHold2(s32 port);
bool isPadHoldUp(s32 port);
bool isPadHoldDown(s32 port);
bool isPadHoldLeft(s32 port);
bool isPadHoldRight(s32 port);
bool isPadHoldLeftUp(s32 port);
bool isPadHoldLeftDown(s32 port);
bool isPadHoldRightUp(s32 port);
bool isPadHoldRightDown(s32 port);
bool isPadHoldHome(s32 port);
bool isPadHoldStart(s32 port);
bool isPadHoldSelect(s32 port);
bool isPadHoldPlus(s32 port);
bool isPadHoldMinus(s32 port);
bool isPadHoldAny(s32 port);
bool isPadHoldAnyWithoutStick(s32 port);
bool isPadHoldTouch();
bool isPadHoldUpLeftStick(s32 port);
bool isPadHoldDownLeftStick(s32 port);
bool isPadHoldLeftLeftStick(s32 port);
bool isPadHoldRightLeftStick(s32 port);
bool isPadHoldUpRightStick(s32 port);
bool isPadHoldDownRightStick(s32 port);
bool isPadHoldLeftRightStick(s32 port);
bool isPadHoldRightRightStick(s32 port);
bool isPadHoldLeftStick(s32 port);
bool isPadHoldRightStick(s32 port);
bool isPadHoldUiCursorUp(s32 port);
bool isPadHoldUiCursorDown(s32 port);
bool isPadHoldUiCursorLeft(s32 port);
bool isPadHoldUiCursorRight(s32 port);

bool isPadRelease(s32, s32);
bool isPadReleaseA(s32 port);
bool isPadReleaseB(s32 port);
bool isPadReleaseX(s32 port);
bool isPadReleaseY(s32 port);
bool isPadReleaseZL(s32 port);
bool isPadReleaseZR(s32 port);
bool isPadReleaseL(s32 port);
bool isPadReleaseR(s32 port);
bool isPadRelease1(s32 port);
bool isPadRelease2(s32 port);
bool isPadReleaseUp(s32 port);
bool isPadReleaseDown(s32 port);
bool isPadReleaseLeft(s32 port);
bool isPadReleaseRight(s32 port);
bool isPadReleaseHome(s32 port);
bool isPadReleaseStart(s32 port);
bool isPadReleaseSelect(s32 port);
bool isPadReleasePlus(s32 port);
bool isPadReleaseMinus(s32 port);
bool isPadReleaseTouch();
bool isPadReleaseUpLeftStick(s32 port);
bool isPadReleaseDownLeftStick(s32 port);
bool isPadReleaseLeftLeftStick(s32 port);
bool isPadReleaseRightLeftStick(s32 port);
bool isPadReleaseUpRightStick(s32 port);
bool isPadReleaseDownRightStick(s32 port);
bool isPadReleaseLeftRightStick(s32 port);
bool isPadReleaseRightRightStick(s32 port);
bool isPadReleaseUiCursorUp(s32 port);  // TODO implement below
bool isPadReleaseUiCursorDown(s32 port);
bool isPadReleaseUiCursorLeft(s32 port);
bool isPadReleaseUiCursorRight(s32 port);

const sead::Vector2f& getLeftStick(s32 port);
const sead::Vector2f& getRightStick(s32 port);

void getPadCrossDir(sead::Vector2f*, s32);
void getPadCrossDirSideways(sead::Vector2f*, s32);

void calcTouchScreenPos(sead::Vector2f*);
void calcTouchLayoutPos(sead::Vector2f*);

bool isTouchPosInRect(const sead::Vector2f&, const sead::Vector2f&);
bool isTouchPosInCircle(const sead::Vector2f&, f32);
bool isTouchPosInCircleByWorldPos(const sead::Vector2f&, const IUseCamera*, f32, f32);
bool isPadTouchRect(f32, f32, f32, f32);

void setPadRepeat(s32, s32, s32, s32);

s32 getPlayerControllerPort(s32 port);
s32 getTouchPanelPort();
s32 getMainControllerPort();
s32 getMainJoyPadDoublePort();
s32 getMainJoyPadSingleRightPort();
s32 getMainJoyPadSingleLeftPort();

bool isSameNpadId(u32, s32);  // TODO implement

}  // namespace al
