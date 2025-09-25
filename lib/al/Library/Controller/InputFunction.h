#pragma once

#include <math/seadVector.h>

namespace sead {
class ControllerBase;
}

namespace al {
class IUseCamera;

bool isPadTypeJoySingle(s32 port = -1);

bool isPadTrigger(s32 port, s32 button);
bool isPadTriggerA(s32 port = -1);
bool isPadTriggerB(s32 port = -1);
bool isPadTriggerX(s32 port = -1);
bool isPadTriggerY(s32 port = -1);
bool isPadTriggerZL(s32 port = -1);
bool isPadTriggerZR(s32 port = -1);
bool isPadTriggerL(s32 port = -1);
bool isPadTriggerR(s32 port = -1);
bool isPadTrigger1(s32 port = -1);
bool isPadTrigger2(s32 port = -1);
bool isPadTriggerUp(s32 port = -1);
bool isPadTriggerDown(s32 port = -1);
bool isPadTriggerLeft(s32 port = -1);
bool isPadTriggerRight(s32 port = -1);
bool isPadTriggerLeftUp(s32 port = -1);
bool isPadTriggerLeftDown(s32 port = -1);
bool isPadTriggerRightUp(s32 port = -1);
bool isPadTriggerRightDown(s32 port = -1);
bool isPadTriggerHome(s32 port = -1);
bool isPadTriggerStart(s32 port = -1);
bool isPadTriggerSelect(s32 port = -1);
bool isPadTriggerPlus(s32 port = -1);
bool isPadTriggerMinus(s32 port = -1);
bool isPadTriggerTouch();
bool isPadTriggerUpLeftStick(s32 port = -1);
bool isPadTriggerDownLeftStick(s32 port = -1);
bool isPadTriggerLeftLeftStick(s32 port = -1);
bool isPadTriggerRightLeftStick(s32 port = -1);
bool isPadTriggerUpRightStick(s32 port = -1);
bool isPadTriggerDownRightStick(s32 port = -1);
bool isPadTriggerLeftRightStick(s32 port = -1);
bool isPadTriggerRightRightStick(s32 port = -1);
bool isPadTriggerAnyABXY(s32 port = -1);
bool isPadTriggerAny(s32 port = -1);
bool isPadTriggerLeftStick(s32 port = -1);
bool isPadTriggerRightStick(s32 port = -1);
bool isPadTriggerPressLeftStick(s32 port = -1);
bool isPadTriggerPressRightStick(s32 port = -1);
bool isPadTriggerUiCursorUp(s32 port = -1);  // TODO implement below
bool isPadTriggerUiCursorDown(s32 port = -1);
bool isPadTriggerUiCursorLeft(s32 port = -1);
bool isPadTriggerUiCursorRight(s32 port = -1);

bool isPadRepeatA(s32 port = -1);
bool isPadRepeatB(s32 port = -1);
bool isPadRepeatX(s32 port = -1);
bool isPadRepeatY(s32 port = -1);
bool isPadRepeatZL(s32 port = -1);
bool isPadRepeatZR(s32 port = -1);
bool isPadRepeatL(s32 port = -1);
bool isPadRepeatR(s32 port = -1);
bool isPadRepeat1(s32 port = -1);
bool isPadRepeat2(s32 port = -1);
bool isPadRepeatUp(s32 port = -1);
bool isPadRepeatDown(s32 port = -1);
bool isPadRepeatLeft(s32 port = -1);
bool isPadRepeatRight(s32 port = -1);
bool isPadRepeatHome(s32 port = -1);
bool isPadRepeatStart(s32 port = -1);
bool isPadRepeatSelect(s32 port = -1);
bool isPadRepeatPlus(s32 port = -1);
bool isPadRepeatMinus(s32 port = -1);
bool isPadRepeatTouch();
bool isPadRepeatUpLeftStick(s32 port = -1);
bool isPadRepeatDownLeftStick(s32 port = -1);
bool isPadRepeatLeftLeftStick(s32 port = -1);
bool isPadRepeatRightLeftStick(s32 port = -1);
bool isPadRepeatUpRightStick(s32 port = -1);
bool isPadRepeatDownRightStick(s32 port = -1);
bool isPadRepeatLeftRightStick(s32 port = -1);
bool isPadRepeatRightRightStick(s32 port = -1);
bool isPadRepeatUiCursorUp(s32 port = -1);  // TODO implement below
bool isPadRepeatUiCursorDown(s32 port = -1);
bool isPadRepeatUiCursorLeft(s32 port = -1);
bool isPadRepeatUiCursorRight(s32 port = -1);

bool isPadHoldPressLeftStick(s32 port = -1);
bool isPadHoldPressRightStick(s32 port = -1);
bool isPadHold(s32 port, s32 button);
bool isPadHoldA(s32 port = -1);
bool isPadHoldB(s32 port = -1);
bool isPadHoldX(s32 port = -1);
bool isPadHoldY(s32 port = -1);
bool isPadHoldZL(s32 port = -1);
bool isPadHoldZR(s32 port = -1);
bool isPadHoldL(s32 port = -1);
bool isPadHoldR(s32 port = -1);
bool isPadHold1(s32 port = -1);
bool isPadHold2(s32 port = -1);
bool isPadHoldUp(s32 port = -1);
bool isPadHoldDown(s32 port = -1);
bool isPadHoldLeft(s32 port = -1);
bool isPadHoldRight(s32 port = -1);
bool isPadHoldLeftUp(s32 port = -1);
bool isPadHoldLeftDown(s32 port = -1);
bool isPadHoldRightUp(s32 port = -1);
bool isPadHoldRightDown(s32 port = -1);
bool isPadHoldHome(s32 port = -1);
bool isPadHoldStart(s32 port = -1);
bool isPadHoldSelect(s32 port = -1);
bool isPadHoldPlus(s32 port = -1);
bool isPadHoldMinus(s32 port = -1);
bool isPadHoldAny(s32 port = -1);
bool isPadHoldAnyWithoutStick(s32 port = -1);
bool isPadHoldTouch();
bool isPadHoldUpLeftStick(s32 port = -1);
bool isPadHoldDownLeftStick(s32 port = -1);
bool isPadHoldLeftLeftStick(s32 port = -1);
bool isPadHoldRightLeftStick(s32 port = -1);
bool isPadHoldUpRightStick(s32 port = -1);
bool isPadHoldDownRightStick(s32 port = -1);
bool isPadHoldLeftRightStick(s32 port = -1);
bool isPadHoldRightRightStick(s32 port = -1);
bool isPadHoldLeftStick(s32 port = -1);
bool isPadHoldRightStick(s32 port = -1);
bool isPadHoldUiCursorUp(s32 port = -1);
bool isPadHoldUiCursorDown(s32 port = -1);
bool isPadHoldUiCursorLeft(s32 port = -1);
bool isPadHoldUiCursorRight(s32 port = -1);

bool isPadRelease(s32 port, s32 button);
bool isPadReleaseA(s32 port = -1);
bool isPadReleaseB(s32 port = -1);
bool isPadReleaseX(s32 port = -1);
bool isPadReleaseY(s32 port = -1);
bool isPadReleaseZL(s32 port = -1);
bool isPadReleaseZR(s32 port = -1);
bool isPadReleaseL(s32 port = -1);
bool isPadReleaseR(s32 port = -1);
bool isPadRelease1(s32 port = -1);
bool isPadRelease2(s32 port = -1);
bool isPadReleaseUp(s32 port = -1);
bool isPadReleaseDown(s32 port = -1);
bool isPadReleaseLeft(s32 port = -1);
bool isPadReleaseRight(s32 port = -1);
bool isPadReleaseHome(s32 port = -1);
bool isPadReleaseStart(s32 port = -1);
bool isPadReleaseSelect(s32 port = -1);
bool isPadReleasePlus(s32 port = -1);
bool isPadReleaseMinus(s32 port = -1);
bool isPadReleaseTouch();
bool isPadReleaseUpLeftStick(s32 port = -1);
bool isPadReleaseDownLeftStick(s32 port = -1);
bool isPadReleaseLeftLeftStick(s32 port = -1);
bool isPadReleaseRightLeftStick(s32 port = -1);
bool isPadReleaseUpRightStick(s32 port = -1);
bool isPadReleaseDownRightStick(s32 port = -1);
bool isPadReleaseLeftRightStick(s32 port = -1);
bool isPadReleaseRightRightStick(s32 port = -1);
bool isPadReleaseUiCursorUp(s32 port = -1);  // TODO implement below
bool isPadReleaseUiCursorDown(s32 port = -1);
bool isPadReleaseUiCursorLeft(s32 port = -1);
bool isPadReleaseUiCursorRight(s32 port = -1);

const sead::Vector2f& getLeftStick(s32 port = -1);
const sead::Vector2f& getRightStick(s32 port = -1);

void getPadCrossDir(sead::Vector2f* dir, s32 port);
void getPadCrossDirSideways(sead::Vector2f* dir, s32 port);

void calcTouchScreenPos(sead::Vector2f* pos);
void calcTouchLayoutPos(sead::Vector2f* pos);

bool isTouchPosInRect(const sead::Vector2f& rect_pos, const sead::Vector2f& size);
bool isTouchPosInCircle(const sead::Vector2f&, f32);
bool isTouchPosInCircleByWorldPos(const sead::Vector2f&, const IUseCamera*, f32, f32);
bool isPadTouchRect(f32, f32, f32, f32);

// TODO: rename parameters
void setPadRepeat(s32 a1, s32 a2, s32 a3, s32 port);

s32 getPlayerControllerPort(s32 playerNo);
s32 getTouchPanelPort();
s32 getMainControllerPort();
s32 getMainJoyPadDoublePort();
s32 getMainJoyPadSingleRightPort();
s32 getMainJoyPadSingleLeftPort();

bool isSameNpadId(u32, s32);  // TODO implement

}  // namespace al
