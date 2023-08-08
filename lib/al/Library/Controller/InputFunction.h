#pragma once

#include <controller/seadController.h>
#include <controller/seadControllerMgr.h>
#include <math/seadVector.h>

namespace al {
class IUseCamera;

bool isValidReplayController(u32);
sead::ControllerBase* getReplayController(u32);  // return type might be wrong

bool isPadTypeJoySingle(int);

bool isPadTrigger(int, int);
bool isPadTriggerA(int);
bool isPadTriggerB(int);
bool isPadTriggerX(int);
bool isPadTriggerY(int);
bool isPadTriggerZL(int);
bool isPadTriggerZR(int);
bool isPadTriggerL(int);
bool isPadTriggerR(int);
bool isPadTrigger1(int);
bool isPadTrigger2(int);
bool isPadTriggerUp(int);
bool isPadTriggerDown(int);
bool isPadTriggerLeft(int);
bool isPadTriggerRight(int);
bool isPadTriggerLeftUp(int);
bool isPadTriggerLeftDown(int);
bool isPadTriggerRightUp(int);
bool isPadTriggerRightDown(int);
bool isPadTriggerHome(int);
bool isPadTriggerStart(int);
bool isPadTriggerSelect(int);
bool isPadTriggerPlus(int);
bool isPadTriggerMinus(int);
bool isPadTriggerTouch();
bool isPadTriggerUpLeftStick(int);
bool isPadTriggerDownLeftStick(int);
bool isPadTriggerLeftLeftStick(int);
bool isPadTriggerRightLeftStick(int);
bool isPadTriggerUpRightStick(int);
bool isPadTriggerDownRightStick(int);
bool isPadTriggerLeftRightStick(int);
bool isPadTriggerRightRightStick(int);
bool isPadTriggerAnyABXY(int);
bool isPadTriggerAny(int);
bool isPadTriggerLeftStick(int);
bool isPadTriggerRightStick(int);
bool isPadTriggerPressLeftStick(int);
bool isPadTriggerPressRightStick(int);
bool isPadTriggerUiCursorUp(int);  // TODO implement below
bool isPadTriggerUiCursorDown(int);
bool isPadTriggerUiCursorLeft(int);
bool isPadTriggerUiCursorRight(int);

bool isPadRepeatA(int);
bool isPadRepeatB(int);
bool isPadRepeatX(int);
bool isPadRepeatY(int);
bool isPadRepeatZL(int);
bool isPadRepeatZR(int);
bool isPadRepeatL(int);
bool isPadRepeatR(int);
bool isPadRepeat1(int);
bool isPadRepeat2(int);
bool isPadRepeatUp(int);
bool isPadRepeatDown(int);
bool isPadRepeatLeft(int);
bool isPadRepeatRight(int);
bool isPadRepeatHome(int);
bool isPadRepeatStart(int);
bool isPadRepeatSelect(int);
bool isPadRepeatPlus(int);
bool isPadRepeatMinus(int);
bool isPadRepeatTouch();
bool isPadRepeatUpLeftStick(int);
bool isPadRepeatDownLeftStick(int);
bool isPadRepeatLeftLeftStick(int);
bool isPadRepeatRightLeftStick(int);
bool isPadRepeatUpRightStick(int);
bool isPadRepeatDownRightStick(int);
bool isPadRepeatLeftRightStick(int);
bool isPadRepeatRightRightStick(int);
bool isPadRepeatUiCursorUp(int);  // TODO implement below
bool isPadRepeatUiCursorDown(int);
bool isPadRepeatUiCursorLeft(int);
bool isPadRepeatUiCursorRight(int);

bool isPadHoldPressLeftStick(int);
bool isPadHoldPressRightStick(int);
bool isPadHold(int, int);
bool isPadHoldA(int);
bool isPadHoldB(int);
bool isPadHoldX(int);
bool isPadHoldY(int);
bool isPadHoldZL(int);
bool isPadHoldZR(int);
bool isPadHoldL(int);
bool isPadHoldR(int);
bool isPadHold1(int);
bool isPadHold2(int);
bool isPadHoldUp(int);
bool isPadHoldDown(int);
bool isPadHoldLeft(int);
bool isPadHoldRight(int);
bool isPadHoldLeftUp(int);
bool isPadHoldLeftDown(int);
bool isPadHoldRightUp(int);
bool isPadHoldRightDown(int);
bool isPadHoldHome(int);
bool isPadHoldStart(int);
bool isPadHoldSelect(int);
bool isPadHoldPlus(int);
bool isPadHoldMinus(int);
bool isPadHoldAny(int);
bool isPadHoldAnyWithoutStick(int);
bool isPadHoldTouch();
bool isPadHoldUpLeftStick(int);
bool isPadHoldDownLeftStick(int);
bool isPadHoldLeftLeftStick(int);
bool isPadHoldRightLeftStick(int);
bool isPadHoldUpRightStick(int);
bool isPadHoldDownRightStick(int);
bool isPadHoldLeftRightStick(int);
bool isPadHoldRightRightStick(int);
bool isPadHoldLeftStick(int);
bool isPadHoldRightStick(int);
bool isPadHoldUiCursorUp(int);
bool isPadHoldUiCursorDown(int);
bool isPadHoldUiCursorLeft(int);
bool isPadHoldUiCursorRight(int);

bool isPadRelease(int, int);
bool isPadReleaseA(int);
bool isPadReleaseB(int);
bool isPadReleaseX(int);
bool isPadReleaseY(int);
bool isPadReleaseZL(int);
bool isPadReleaseZR(int);
bool isPadReleaseL(int);
bool isPadReleaseR(int);
bool isPadRelease1(int);
bool isPadRelease2(int);
bool isPadReleaseUp(int);
bool isPadReleaseDown(int);
bool isPadReleaseLeft(int);
bool isPadReleaseRight(int);
bool isPadReleaseHome(int);
bool isPadReleaseStart(int);
bool isPadReleaseSelect(int);
bool isPadReleasePlus(int);
bool isPadReleaseMinus(int);
bool isPadReleaseTouch();
bool isPadReleaseUpLeftStick(int);
bool isPadReleaseDownLeftStick(int);
bool isPadReleaseLeftLeftStick(int);
bool isPadReleaseRightLeftStick(int);
bool isPadReleaseUpRightStick(int);
bool isPadReleaseDownRightStick(int);
bool isPadReleaseLeftRightStick(int);
bool isPadReleaseRightRightStick(int);
bool isPadReleaseUiCursorUp(int);  // TODO implement below
bool isPadReleaseUiCursorDown(int);
bool isPadReleaseUiCursorLeft(int);
bool isPadReleaseUiCursorRight(int);

const sead::Vector2f& getLeftStick(int);
const sead::Vector2f& getRightStick(int);

void getPadCrossDir(sead::Vector2f*, int);
void getPadCrossDirSideways(sead::Vector2f*, int);

void calcTouchScreenPos(sead::Vector2f*);
void calcTouchLayoutPos(sead::Vector2f*);

bool isTouchPosInRect(const sead::Vector2f&, const sead::Vector2f&);
bool isTouchPosInCircle(const sead::Vector2f&, float);
bool isTouchPosInCircleByWorldPos(const sead::Vector2f&, const al::IUseCamera*, float, float);
bool isPadTouchRect(float, float, float, float);

void setPadRepeat(int, int, int, int);

int getPlayerControllerPort(int);
int getTouchPanelPort();
int getMainControllerPort();
int getMainJoyPadDoublePort();
int getMainJoyPadSingleRightPort();
int getMainJoyPadSingleLeftPort();

bool isSameNpadId(u32, int);  // TODO implement

}  // namespace al
