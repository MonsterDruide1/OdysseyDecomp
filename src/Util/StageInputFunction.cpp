#include "Util/StageInputFunction.h"

#include <controller/nin/seadNinJoyNpadDevice.h>
#include <controller/seadControllerMgr.h>
#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Controller/GamePadSystem.h"
#include "Library/Controller/InputFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutSceneInfo.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneObjUtil.h"

#include "System/GameConfigData.h"
#include "System/GameDataHolder.h"
#include "Util/InputInterruptTutorialUtil.h"

namespace {

constexpr s32 cInvalidPadPlayStyle = 5;

bool isTriggerMapOpenSeparatePlay() {
    s32 port0 = al::getPlayerControllerPort(0);
    s32 port1 = al::getPlayerControllerPort(1);

    if ((al::isPadTypeJoyRight(port0) && al::isPadTypeJoyRight(port1)) ||
        (al::isPadTypeJoyLeft(port0) && al::isPadTypeJoyLeft(port1))) {
        if (al::isPadTriggerMinus(port1))
            return true;
        return al::isPadTriggerPlus(port1);
    }

    if (al::isPadTriggerMinus(port0))
        return true;
    return al::isPadTriggerMinus(port1);
}

sead::NinJoyNpadDevice* getNinJoyNpadDevice() {
    return sead::ControllerMgr::instance()->getControlDeviceAs<sead::NinJoyNpadDevice*>();
}

}  // namespace

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder* user) {
    return al::getSceneObj<GameDataHolder>(user)->isSeparatePlay();
}

void changeSeparatePlayMode(al::Scene* scene, bool isSeparatePlay) {
    if (isSeparatePlay) {
        changeSeparatePlayTutorial(scene);
        alCameraFunction::onSeparatePlayMode(scene);
    } else {
        changeSinglePlayTutorial(scene);
        alCameraFunction::offSeparatePlayMode(scene);
    }

    al::getSceneObj<GameDataHolder>(scene)->setSeparatePlay(isSeparatePlay);
}

void setSeparatePlayMode(al::Scene* scene, bool isSeparatePlay) {
    if (isSeparatePlay)
        return alCameraFunction::onSeparatePlayMode(scene);
    return alCameraFunction::offSeparatePlayMode(scene);
}

bool isTriggerUiDecide(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerA(al::getPlayerControllerPort(0)))
            return true;
        return al::isPadTriggerA(al::getPlayerControllerPort(1));
    }
    return al::isPadTriggerA(al::getMainControllerPort());
}

bool isTriggerUiDecide(const GameDataHolder* gameDataHolder) {
    if (gameDataHolder->isSeparatePlay()) {
        if (al::isPadTriggerA(al::getPlayerControllerPort(0)))
            return true;
        return al::isPadTriggerA(al::getPlayerControllerPort(1));
    }
    return al::isPadTriggerA(al::getMainControllerPort());
}

bool isTriggerUiCancel(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerB(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerB(port);
}

bool isTriggerUiSelect(const al::IUseSceneObjHolder* user) {
    s32 port0 = al::getPlayerControllerPort(0);
    s32 port1 = al::getPlayerControllerPort(1);

    if (al::isPadTypeJoyRight(port0) || !al::isPadTypeJoyLeft(port1))
        return al::isPadTriggerPlus(port0);
    return al::isPadTriggerMinus(port1);
}

bool isTriggerUiPause(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        s32 port0 = al::getPlayerControllerPort(0);
        s32 port1 = al::getPlayerControllerPort(1);

        if ((al::isPadTypeJoyRight(port0) && al::isPadTypeJoyRight(port1)) ||
            (al::isPadTypeJoyLeft(port0) && al::isPadTypeJoyLeft(port1))) {
            if (al::isPadTriggerMinus(port0))
                return true;
            return al::isPadTriggerPlus(port0);
        }

        if (al::isPadTriggerPlus(port0))
            return true;
        return al::isPadTriggerPlus(port1);
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerPlus(al::getPlayerControllerPort(0));
    return al::isPadTriggerPlus(al::getMainControllerPort());
}

bool isTriggerUiLeft(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerUiCursorLeft(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerUiCursorLeft(port);
}

bool isTriggerUiRight(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerUiCursorRight(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerUiCursorRight(port);
}

bool isTriggerUiUp(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerUiCursorUp(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerUiCursorUp(port);
}

bool isTriggerUiDown(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerUiCursorDown(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerUiCursorDown(port);
}

bool isTriggerUiR(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerR(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerR(port);
}

bool isTriggerUiL(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerL(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerL(port);
}

bool isTriggerUiZR(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerZR(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerZR(port);
}

bool isTriggerUiZL(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerZL(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerZL(port);
}

bool isTriggerUiX(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerX(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerX(port);
}

bool isTriggerUiY(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerY(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerY(port);
}

bool isTriggerUiAnyABXY(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerAnyABXY(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadTriggerAnyABXY(port);
}

bool isRepeatUiUp(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadRepeatUiCursorUp(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadRepeatUiCursorUp(port);
}

bool isRepeatUiDown(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadRepeatUiCursorDown(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadRepeatUiCursorDown(port);
}

bool isRepeatUiRight(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadRepeatUiCursorRight(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadRepeatUiCursorRight(port);
}

bool isRepeatUiLeft(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadRepeatUiCursorLeft(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadRepeatUiCursorLeft(port);
}

bool isHoldUiDecide(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldA(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldA(port);
}

bool isHoldUiCancel(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldB(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldB(port);
}

bool isHoldUiSelect(const al::IUseSceneObjHolder* user) {
    s32 port0 = al::getPlayerControllerPort(0);
    s32 port1 = al::getPlayerControllerPort(1);
    bool isPort0JoyRight = al::isPadTypeJoyRight(port0);
    bool isPort1JoyRight = al::isPadTypeJoyRight(port1);

    if (isPort0JoyRight) {
        if (isPort1JoyRight)
            return al::isPadTriggerPlus(port0);
        return al::isPadTriggerMinus(port1);
    }

    return al::isPadTriggerMinus(port0);
}

bool isHoldUiL(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldL(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldL(port);
}

bool isHoldUiR(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldR(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldR(port);
}

bool isHoldUiY(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldY(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldY(port);
}

bool isHoldUiUp(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldUiCursorUp(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldUiCursorUp(port);
}

bool isHoldUiDown(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldUiCursorDown(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldUiCursorDown(port);
}

bool isHoldUiLeft(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldUiCursorLeft(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldUiCursorLeft(port);
}

bool isHoldUiRight(const al::IUseSceneObjHolder* user) {
    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldUiCursorRight(al::getPlayerControllerPort(0)))
            return true;
        port = al::getPlayerControllerPort(1);
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            port = al::getPlayerControllerPort(0);
        else
            port = al::getMainControllerPort();
    }

    return al::isPadHoldUiCursorRight(port);
}

bool isHoldSeparatePlayStart1P(const al::IUseSceneObjHolder* user) {
    return isTriggerUiX(user);
}

bool isHoldSeparatePlayStart2P(const al::IUseSceneObjHolder* user) {
    return isTriggerMapOpen(user);
}

bool isTriggerMapOpen(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return isTriggerMapOpenSeparatePlay();

    bool isPadTriggerUp = false;
    if (isUseOpenListAdditionalButton(user)) {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            isPadTriggerUp = al::isPadTriggerUp(al::getPlayerControllerPort(0));
        else
            isPadTriggerUp = al::isPadTriggerUp(al::getMainControllerPort());
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return isPadTriggerUp | al::isPadTriggerMinus(al::getPlayerControllerPort(0));
    return isPadTriggerUp | al::isPadTriggerMinus(al::getMainControllerPort());
}

bool isHoldSeparatePlayEnd(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadTriggerA(al::getPlayerControllerPort(0)))
            return true;
        return al::isPadTriggerA(al::getPlayerControllerPort(1));
    }
    return al::isPadTriggerA(al::getMainControllerPort());
}

sead::Vector2f getUiLeftStick(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        s32 port0 = al::getPlayerControllerPort(0);
        sead::Vector2f stick = {0.0f, 0.0f};

        if (!al::isPadTypeJoySingle(port0) || !al::isPadHoldX(port0))
            stick = al::getLeftStick(port0);

        f32 length = sead::Mathf::sqrt(stick.x * stick.x + stick.y * stick.y);
        if (al::isNearZero(length, 0.001f)) {
            s32 port1 = al::getPlayerControllerPort(1);
            if (!al::isPadTypeJoySingle(port1) || !al::isPadHoldX(port1))
                stick = al::getLeftStick(port1);
        }

        return stick;
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::getLeftStick(al::getPlayerControllerPort(0));
    return al::getLeftStick(al::getMainControllerPort());
}

sead::Vector2f getUiRightStick(const al::IUseSceneObjHolder* user) {
    sead::Vector2f stick;
    f32* stickY;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        stickY = &stick.y;
        stick = {0.0f, 0.0f};
        s32 port0 = al::getPlayerControllerPort(0);

        if (al::isPadTypeJoySingle(port0)) {
            if (al::isPadHoldX(port0))
                stick = al::getLeftStick(port0);
        } else {
            stick = al::getRightStick(port0);
        }

        if (al::isNearZero(stick, 0.001f)) {
            s32 port1 = al::getPlayerControllerPort(1);
            if (al::isPadTypeJoySingle(port1)) {
                if (al::isPadHoldX(port1))
                    stick = al::getLeftStick(port1);
            } else {
                stick = al::getRightStick(port1);
            }
        }
    } else {
        if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
            stick.set(al::getRightStick(al::getPlayerControllerPort(0)));
        else
            stick.set(al::getRightStick(al::getMainControllerPort()));
        stickY = &stick.y;
    }

    return {stick.x, *stickY};
}

bool isTriggerMapClose(const al::IUseSceneObjHolder* user) {
    if (isTriggerMapOpen(user))
        return true;
    return isTriggerUiCancel(user);
}

bool isTriggerCollectionListOpenInMap(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        s32 port0 = al::getPlayerControllerPort(0);
        s32 port1 = al::getPlayerControllerPort(1);
        if (al::isPadTriggerY(port0))
            return true;
        return al::isPadTriggerY(port1);
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerY(al::getPlayerControllerPort(0));
    return al::isPadTriggerY(al::getMainControllerPort());
}

bool isTriggerCollectionListCloseInMap(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        s32 port0 = al::getPlayerControllerPort(0);
        s32 port1 = al::getPlayerControllerPort(1);
        if (al::isPadTriggerY(port0) || al::isPadTriggerY(port1) || isTriggerMapOpenSeparatePlay())
            return true;
        return isTriggerUiCancel(user);
    }

    s32 port;
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        port = al::getPlayerControllerPort(0);
    else
        port = al::getMainControllerPort();
    if (al::isPadTriggerY(port))
        return true;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        port = al::getPlayerControllerPort(0);
    else
        port = al::getMainControllerPort();
    if (al::isPadTriggerMinus(port))
        return true;

    return isTriggerUiCancel(user);
}

bool isTriggerAppearSkipGuide(const al::IUseSceneObjHolder* user) {
    return isTriggerUiPause(user);
}

bool isTriggerUiRacePause(const al::IUseSceneObjHolder* user) {
    u8 isSeparate = al::getSceneObj<GameDataHolder>(user)->getSeparatePlayFlag();
    bool isPause = isTriggerUiPause(user);
    bool isSeparateBool = isSeparate != 0;
    bool result = isPause | isSeparateBool;
    if (isSeparate)
        if (!isPause)
            return isTriggerMapOpen(user);
    return result;
}

bool isTriggerSnapShotMode(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return false;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerDown(al::getPlayerControllerPort(0));
    return al::isPadTriggerDown(al::getMainControllerPort());
}

bool isTriggerAmiiboMode(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return false;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerRight(al::getPlayerControllerPort(0));
    return al::isPadTriggerRight(al::getMainControllerPort());
}

bool isHoldAmiiboMode(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return false;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadHoldRight(al::getPlayerControllerPort(0));
    return al::isPadHoldRight(al::getMainControllerPort());
}

bool isTriggerToggleSnapShotModeLayout(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return false;

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerY(al::getPlayerControllerPort(0));
    return al::isPadTriggerY(al::getMainControllerPort());
}

bool isTriggerAppearSnapShotLayout(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldAny(al::getPlayerControllerPort(0)))
            return true;
    } else if (al::isPadHoldAny(al::getMainControllerPort())) {
        return true;
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay()) {
        if (al::isPadHoldRightStick(al::getPlayerControllerPort(0)))
            return true;
    } else if (al::isPadHoldRightStick(al::getMainControllerPort())) {
        return true;
    }

    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadHoldLeftStick(al::getPlayerControllerPort(0));
    return al::isPadHoldLeftStick(al::getMainControllerPort());
}

bool isTriggerIncrementPostProcessingFilterPreset(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerRight(al::getPlayerControllerPort(0));
    return al::isPadTriggerRight(al::getMainControllerPort());
}

bool isTriggerDecrementPostProcessingFilterPreset(const al::IUseSceneObjHolder* user) {
    if (al::getSceneObj<GameDataHolder>(user)->isSeparatePlay())
        return al::isPadTriggerLeft(al::getPlayerControllerPort(0));
    return al::isPadTriggerLeft(al::getMainControllerPort());
}

void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LiveActor* actor) {
    const al::GamePadSystem* gamePadSystem = actor->getSceneInfo()->gamePadSystem;
    s32 controllerCount = gamePadSystem->getMinControllerCount();

    info->mControllerCount = controllerCount;
    info->mPort0PlayStyle = gamePadSystem->getPadPlayStyle(0);
    if (controllerCount >= 2)
        info->mPort1PlayStyle = gamePadSystem->getPadPlayStyle(1);
    else
        info->mPort1PlayStyle = cInvalidPadPlayStyle;
}

void recordGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LayoutActor* layoutActor) {
    const al::GamePadSystem* gamePadSystem = layoutActor->getLayoutSceneInfo()->getGamePadSystem();
    s32 controllerCount = gamePadSystem->getMinControllerCount();

    info->mControllerCount = controllerCount;
    info->mPort0PlayStyle = gamePadSystem->getPadPlayStyle(0);
    if (controllerCount >= 2)
        info->mPort1PlayStyle = gamePadSystem->getPadPlayStyle(1);
    else
        info->mPort1PlayStyle = cInvalidPadPlayStyle;
}

bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LiveActor* actor) {
    const al::GamePadSystem* gamePadSystem = actor->getSceneInfo()->gamePadSystem;
    s32 controllerCount = gamePadSystem->getMinControllerCount();
    s32 port0PlayStyle = gamePadSystem->getPadPlayStyle(0);
    s32 port1PlayStyle;
    if (controllerCount >= 2)
        port1PlayStyle = gamePadSystem->getPadPlayStyle(1);
    else
        port1PlayStyle = cInvalidPadPlayStyle;

    if (info->mControllerCount == controllerCount && info->mPort0PlayStyle == port0PlayStyle &&
        info->mPort1PlayStyle == port1PlayStyle)
        return false;

    info->mControllerCount = controllerCount;
    info->mPort0PlayStyle = port0PlayStyle;
    info->mPort1PlayStyle = port1PlayStyle;
    return true;
}

bool tryUpdateGamePadPlayStyleInfo(GamePadPlayStyleInfo* info, const al::LayoutActor* layoutActor) {
    const al::GamePadSystem* gamePadSystem = layoutActor->getLayoutSceneInfo()->getGamePadSystem();
    s32 controllerCount = gamePadSystem->getMinControllerCount();
    s32 port0PlayStyle = gamePadSystem->getPadPlayStyle(0);
    s32 port1PlayStyle;
    if (controllerCount >= 2)
        port1PlayStyle = gamePadSystem->getPadPlayStyle(1);
    else
        port1PlayStyle = cInvalidPadPlayStyle;

    if (info->mControllerCount == controllerCount && info->mPort0PlayStyle == port0PlayStyle &&
        info->mPort1PlayStyle == port1PlayStyle)
        return false;

    info->mControllerCount = controllerCount;
    info->mPort0PlayStyle = port0PlayStyle;
    info->mPort1PlayStyle = port1PlayStyle;
    return true;
}

}  // namespace rs

namespace ControllerAppletFunction {

bool tryReconnectGamePad(al::GamePadSystem* gamePadSystem) {
    if (!gamePadSystem->isDisconnectPlayable())
        return true;

    if (gamePadSystem->getMinControllerCount() == 1)
        return al::tryCallControllerApplet(gamePadSystem, 0, 1);
    return al::tryCallControllerApplet(gamePadSystem, gamePadSystem->getMaxControllerCount(),
                                       gamePadSystem->getMinControllerCount());
}

bool connectControllerSinglePlay(al::GamePadSystem* gamePadSystem) {
    sead::NinJoyNpadDevice* npadDevice = getNinJoyNpadDevice();
    npadDevice->mergeSingleJoyAsDualJoy(0, 1);
    npadDevice->setSupportedNpadStyleSet({{7}});

    if (al::tryCallControllerApplet(gamePadSystem, 0, 1)) {
        gamePadSystem->changeSinglePlayMode();
        return true;
    }

    npadDevice->setSupportedNpadStyleSet({{29}});
    return false;
}

bool connectControllerSeparatePlay(al::GamePadSystem* gamePadSystem) {
    sead::NinJoyNpadDevice* npadDevice = getNinJoyNpadDevice();
    npadDevice->setSupportedNpadStyleSet({{29}});

    if (al::tryCallControllerApplet(gamePadSystem, 2, 2)) {
        gamePadSystem->changeMultiPlayMode(2, 2);
        return true;
    }

    npadDevice->mergeSingleJoyAsDualJoy(0, 1);
    npadDevice->setSupportedNpadStyleSet({{7}});
    return false;
}

}  // namespace ControllerAppletFunction
