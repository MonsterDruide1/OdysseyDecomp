#include "al/util/InputUtil.h"

#include "al/util/OtherUtil.h"

namespace al {

sead::ControllerBase* getController_(int port) {
    return al::isValidReplayController(port) ? getReplayController(port) :
                                               sead::ControllerMgr::instance()->getController(port);
}

sead::ControllerBase* getController(int port) {
    if (port == -1) {
        port = getMainControllerPort();
    }

    return getController_(port);
}

bool isPadTrigger(int port, int button) {
    return getController(port)->getButtonsTrigger() & button;
}

bool isPadTriggerA(int port) {
    return isPadTrigger(port, 1);
}
bool isPadTriggerB(int port) {
    return isPadTrigger(port, 1 << 1);
}
bool isPadTriggerX(int port) {
    return isPadTrigger(port, 1 << 3);
}
bool isPadTriggerY(int port) {
    return isPadTrigger(port, 1 << 4);
}
bool isPadTriggerZL(int port) {
    return isPadTrigger(port, 1 << 2);
}
bool isPadTriggerZR(int port) {
    return isPadTrigger(port, 1 << 5);
}
bool isPadTriggerL(int port) {
    return isPadTrigger(port, 1 << 13);
}
bool isPadTriggerR(int port) {
    return isPadTrigger(port, 1 << 14);
}
bool isPadTrigger1(int port) {
    return isPadTrigger(port, 1 << 7);
}
bool isPadTrigger2(int port) {
    return isPadTrigger(port, 1 << 6);
}
bool isPadTriggerUp(int port) {
    return isPadTrigger(port, 1 << 16);
}
bool isPadTriggerDown(int port) {
    return isPadTrigger(port, 1 << 17);
}
bool isPadTriggerLeft(int port) {
    return isPadTrigger(port, 1 << 18);
}
bool isPadTriggerRight(int port) {
    return isPadTrigger(port, 1 << 19);
}
bool isPadTriggerLeftUp(int port) {
    return isPadHoldLeftUp(port) && (getController(port)->getButtonsTrigger() & 0x50000);
}
bool isPadTriggerLeftDown(int port) {
    return isPadHoldLeftDown(port) && (getController(port)->getButtonsTrigger() & 0x60000);
}
bool isPadTriggerRightUp(int port) {
    return isPadHoldRightUp(port) && (getController(port)->getButtonsTrigger() & 0x90000);
}
bool isPadTriggerRightDown(int port) {
    return isPadHoldRightDown(port) && (getController(port)->getButtonsTrigger() & 0xA0000);
}
bool isPadTriggerHome(int port) {
    return isPadTrigger(port, 1 << 8);
}
bool isPadTriggerStart(int port) {
    return isPadTrigger(port, 1 << 11);
}
bool isPadTriggerSelect(int port) {
    return isPadTrigger(port, 1 << 12);
}
bool isPadTriggerPlus(int port) {
    return isPadTrigger(port, 1 << 10);
}
bool isPadTriggerMinus(int port) {
    return isPadTrigger(port, 1 << 9);
}
bool isPadTriggerTouch() {
    return isPadTrigger(getTouchPanelPort(), 1 << 15);
}
bool isPadTriggerUpLeftStick(int port) {
    return isPadTrigger(port, 1 << 20);
}
bool isPadTriggerDownLeftStick(int port) {
    return isPadTrigger(port, 1 << 21);
}
bool isPadTriggerLeftLeftStick(int port) {
    return isPadTrigger(port, 1 << 22);
}
bool isPadTriggerRightLeftStick(int port) {
    return isPadTrigger(port, 1 << 23);
}
bool isPadTriggerUpRightStick(int port) {
    return isPadTrigger(port, 1 << 24);
}
bool isPadTriggerDownRightStick(int port) {
    return isPadTrigger(port, 1 << 25);
}
bool isPadTriggerLeftRightStick(int port) {
    return isPadTrigger(port, 1 << 26);
}
bool isPadTriggerRightRightStick(int port) {
    return isPadTrigger(port, 1 << 27);
}
bool isPadTriggerAnyABXY(int port) {
    return isPadTriggerA(port) || isPadTriggerB(port) || isPadTriggerX(port) || isPadTriggerY(port);
}
bool isPadTriggerAny(int port) {
    return isPadTrigger(port, 0xFFF7FFF);
}
bool isPadTriggerLeftStick(int port) {
    return isPadTrigger(port, 0xF00000);
}
bool isPadTriggerRightStick(int port) {
    return isPadTrigger(port, 0xF000000);
}
bool isPadTriggerPressLeftStick(int port) {
    return isPadTrigger1(port);
}
bool isPadTriggerPressRightStick(int port) {
    return isPadTrigger2(port);
}

bool isPadRepeat(int port, int button) {
    sead::ControllerBase* controller = getController(port);
    return (controller->getButtonsTrigger() | controller->getButtonsRepeat()) & button;
}
bool isPadRepeatA(int port) {
    return isPadRepeat(port, 1);
}
bool isPadRepeatB(int port) {
    return isPadRepeat(port, 1 << 1);
}
bool isPadRepeatX(int port) {
    return isPadRepeat(port, 1 << 3);
}
bool isPadRepeatY(int port) {
    return isPadRepeat(port, 1 << 4);
}
bool isPadRepeatZL(int port) {
    return isPadRepeat(port, 1 << 2);
}
bool isPadRepeatZR(int port) {
    return isPadRepeat(port, 1 << 5);
}
bool isPadRepeatL(int port) {
    return isPadRepeat(port, 1 << 13);
}
bool isPadRepeatR(int port) {
    return isPadRepeat(port, 1 << 14);
}
bool isPadRepeat1(int port) {
    return isPadRepeat(port, 1 << 7);
}
bool isPadRepeat2(int port) {
    return isPadRepeat(port, 1 << 6);
}
bool isPadRepeatUp(int port) {
    return isPadRepeat(port, 1 << 16);
}
bool isPadRepeatDown(int port) {
    return isPadRepeat(port, 1 << 17);
}
bool isPadRepeatLeft(int port) {
    return isPadRepeat(port, 1 << 18);
}
bool isPadRepeatRight(int port) {
    return isPadRepeat(port, 1 << 19);
}
bool isPadRepeatHome(int port) {
    return isPadRepeat(port, 1 << 8);
}
bool isPadRepeatStart(int port) {
    return isPadRepeat(port, 1 << 11);
}
bool isPadRepeatSelect(int port) {
    return isPadRepeat(port, 1 << 12);
}
bool isPadRepeatPlus(int port) {
    return isPadRepeat(port, 1 << 10);
}
bool isPadRepeatMinus(int port) {
    return isPadRepeat(port, 1 << 9);
}
bool isPadRepeatTouch() {
    return isPadRepeat(getTouchPanelPort(), 1 << 15);
}
bool isPadRepeatUpLeftStick(int port) {
    return isPadRepeat(port, 1 << 20);
}
bool isPadRepeatDownLeftStick(int port) {
    return isPadRepeat(port, 1 << 21);
}
bool isPadRepeatLeftLeftStick(int port) {
    return isPadRepeat(port, 1 << 22);
}
bool isPadRepeatRightLeftStick(int port) {
    return isPadRepeat(port, 1 << 23);
}
bool isPadRepeatUpRightStick(int port) {
    return isPadRepeat(port, 1 << 24);
}
bool isPadRepeatDownRightStick(int port) {
    return isPadRepeat(port, 1 << 25);
}
bool isPadRepeatLeftRightStick(int port) {
    return isPadRepeat(port, 1 << 26);
}
bool isPadRepeatRightRightStick(int port) {
    return isPadRepeat(port, 1 << 27);
}

bool isPadHoldPressLeftStick(int port) {
    return isPadHold1(port);
}
bool isPadHoldPressRightStick(int port) {
    return isPadHold2(port);
}
bool isPadHold(int port, int button) {
    return getController(port)->getButtonsHold() & button;
}
bool isPadHoldA(int port) {
    return isPadHold(port, 1);
}
bool isPadHoldB(int port) {
    return isPadHold(port, 1 << 1);
}
bool isPadHoldX(int port) {
    return isPadHold(port, 1 << 3);
}
bool isPadHoldY(int port) {
    return isPadHold(port, 1 << 4);
}
bool isPadHoldZL(int port) {
    return isPadHold(port, 1 << 2);
}
bool isPadHoldZR(int port) {
    return isPadHold(port, 1 << 5);
}
bool isPadHoldL(int port) {
    return isPadHold(port, 1 << 13);
}
bool isPadHoldR(int port) {
    return isPadHold(port, 1 << 14);
}
bool isPadHold1(int port) {
    return isPadHold(port, 1 << 7);
}
bool isPadHold2(int port) {
    return isPadHold(port, 1 << 6);
}
bool isPadHoldUp(int port) {
    return isPadHold(port, 1 << 16);
}
bool isPadHoldDown(int port) {
    return isPadHold(port, 1 << 17);
}
bool isPadHoldLeft(int port) {
    return isPadHold(port, 1 << 18);
}
bool isPadHoldRight(int port) {
    return isPadHold(port, 1 << 19);
}
bool isPadHoldLeftUp(int port) {
    return (getController(port)->getButtonsHold() & 0x50000) == 0x50000;
}
bool isPadHoldLeftDown(int port) {
    return (getController(port)->getButtonsHold() & 0x60000) == 0x60000;
}
bool isPadHoldRightUp(int port) {
    return (getController(port)->getButtonsHold() & 0x90000) == 0x90000;
}
bool isPadHoldRightDown(int port) {
    return (getController(port)->getButtonsHold() & 0xA0000) == 0xA0000;
}
bool isPadHoldHome(int port) {
    return isPadHold(port, 1 << 8);
}
bool isPadHoldStart(int port) {
    return isPadHold(port, 1 << 11);
}
bool isPadHoldSelect(int port) {
    return isPadHold(port, 1 << 12);
}
bool isPadHoldPlus(int port) {
    return isPadHold(port, 1 << 10);
}
bool isPadHoldMinus(int port) {
    return isPadHold(port, 1 << 9);
}
bool isPadHoldAny(int port) {
    return isPadHold(port, 0xFFF7FFF);
}
bool isPadHoldAnyWithoutStick(int port) {
    return isPadHold(port, 0xF7FFF);
}
bool isPadHoldTouch() {
    return isPadHold(getTouchPanelPort(), 1 << 15);
}
bool isPadHoldUpLeftStick(int port) {
    return isPadHold(port, 1 << 20);
}
bool isPadHoldDownLeftStick(int port) {
    return isPadHold(port, 1 << 21);
}
bool isPadHoldLeftLeftStick(int port) {
    return isPadHold(port, 1 << 22);
}
bool isPadHoldRightLeftStick(int port) {
    return isPadHold(port, 1 << 23);
}
bool isPadHoldUpRightStick(int port) {
    return isPadHold(port, 1 << 24);
}
bool isPadHoldDownRightStick(int port) {
    return isPadHold(port, 1 << 25);
}
bool isPadHoldLeftRightStick(int port) {
    return isPadHold(port, 1 << 26);
}
bool isPadHoldRightRightStick(int port) {
    return isPadHold(port, 1 << 27);
}
bool isPadHoldLeftStick(int port) {
    return isPadHold(port, 0xF00000);
}
bool isPadHoldRightStick(int port) {
    return isPadHold(port, 0xF000000);
}

bool isPadRelease(int port, int button) {
    return getController(port)->getButtonsRelease() & button;
}
bool isPadReleaseA(int port) {
    return isPadRelease(port, 1);
}
bool isPadReleaseB(int port) {
    return isPadRelease(port, 1 << 1);
}
bool isPadReleaseX(int port) {
    return isPadRelease(port, 1 << 3);
}
bool isPadReleaseY(int port) {
    return isPadRelease(port, 1 << 4);
}
bool isPadReleaseZL(int port) {
    return isPadRelease(port, 1 << 2);
}
bool isPadReleaseZR(int port) {
    return isPadRelease(port, 1 << 5);
}
bool isPadReleaseL(int port) {
    return isPadRelease(port, 1 << 13);
}
bool isPadReleaseR(int port) {
    return isPadRelease(port, 1 << 14);
}
bool isPadRelease1(int port) {
    return isPadRelease(port, 1 << 7);
}
bool isPadRelease2(int port) {
    return isPadRelease(port, 1 << 6);
}
bool isPadReleaseUp(int port) {
    return isPadRelease(port, 1 << 16);
}
bool isPadReleaseDown(int port) {
    return isPadRelease(port, 1 << 17);
}
bool isPadReleaseLeft(int port) {
    return isPadRelease(port, 1 << 18);
}
bool isPadReleaseRight(int port) {
    return isPadRelease(port, 1 << 19);
}
bool isPadReleaseHome(int port) {
    return isPadRelease(port, 1 << 8);
}
bool isPadReleaseStart(int port) {
    return isPadRelease(port, 1 << 11);
}
bool isPadReleaseSelect(int port) {
    return isPadRelease(port, 1 << 12);
}
bool isPadReleasePlus(int port) {
    return isPadRelease(port, 1 << 10);
}
bool isPadReleaseMinus(int port) {
    return isPadRelease(port, 1 << 9);
}
bool isPadReleaseTouch() {
    return isPadRelease(getTouchPanelPort(), 1 << 15);
}
bool isPadReleaseUpLeftStick(int port) {
    return isPadRelease(port, 1 << 20);
}
bool isPadReleaseDownLeftStick(int port) {
    return isPadRelease(port, 1 << 21);
}
bool isPadReleaseLeftLeftStick(int port) {
    return isPadRelease(port, 1 << 22);
}
bool isPadReleaseRightLeftStick(int port) {
    return isPadRelease(port, 1 << 23);
}
bool isPadReleaseUpRightStick(int port) {
    return isPadRelease(port, 1 << 24);
}
bool isPadReleaseDownRightStick(int port) {
    return isPadRelease(port, 1 << 25);
}
bool isPadReleaseLeftRightStick(int port) {
    return isPadRelease(port, 1 << 26);
}
bool isPadReleaseRightRightStick(int port) {
    return isPadRelease(port, 1 << 27);
}

const sead::Vector2f& getLeftStick(int port) {
    return getController(port)->getLeftJoy();
}
const sead::Vector2f& getRightStick(int port) {
    return getController(port)->getRightJoy();
}

void getPadCrossDir(sead::Vector2f* vec, int port) {
    vec->x = 0;
    vec->y = 0;
    if (isPadHoldUp(port))
        vec->y = 1;
    if (isPadHoldDown(port))
        vec->y = -1;
    if (isPadHoldLeft(port))
        vec->x = -1;
    if (isPadHoldRight(port))
        vec->x = 1;
}
void getPadCrossDirSideways(sead::Vector2f* vec, int port) {
    vec->x = 0;
    vec->y = 0;
    if (isPadHoldUp(port))
        vec->x = -1;
    if (isPadHoldDown(port))
        vec->x = 1;
    if (isPadHoldLeft(port))
        vec->y = -1;
    if (isPadHoldRight(port))
        vec->y = 1;
}

#ifdef NON_MATCHING
void calcTouchScreenPos(sead::Vector2f* vec) {
    *vec = getController(getTouchPanelPort())
               ->getTouchScreenPos();  // uses w8 for storage instead and inserts another write at
                                       // +4 bytes
}
#endif

void calcTouchLayoutPos(sead::Vector2f*) {}

bool isTouchPosInRect(const sead::Vector2f& rect_pos, const sead::Vector2f& size) {
    sead::Vector2f pos;
    calcTouchScreenPos(&pos);

    if (rect_pos.x > pos.x)
        return false;
    if (pos.x >= (rect_pos.x + size.x))
        return false;
    if (rect_pos.y > pos.y)
        return false;
    if (pos.y > rect_pos.y + size.y)
        return false;

    return true;
}

void setPadRepeat(int a1, int a2, int a3, int port) {
    getController(port)->setPadRepeat(a1, a2, a3);
}

int getPlayerControllerPort(int playerNo) {
    auto* manager = sead::ControllerMgr::instance();
    sead::Controller* controller =
        manager->getControllerByOrder(sead::ControllerDefine::ControllerId::_15, playerNo);
    return manager->findControllerPort(controller);
}
int getTouchPanelPort() {
    auto* manager = sead::ControllerMgr::instance();
    sead::Controller* controller =
        manager->getControllerByOrder(sead::ControllerDefine::ControllerId::_16, 0);
    return manager->findControllerPort(controller);
}
int getMainControllerPort() {
    return getPlayerControllerPort(0);
}
int getMainJoyPadDoublePort() {
    return getMainControllerPort();
}
int getMainJoyPadSingleRightPort() {
    return 1;
}
int getMainJoyPadSingleLeftPort() {
    return 2;
}

}  // namespace al
