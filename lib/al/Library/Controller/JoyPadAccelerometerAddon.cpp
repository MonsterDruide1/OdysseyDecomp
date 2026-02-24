#include "Library/Controller/JoyPadAccelerometerAddon.h"

#include <controller/nin/seadNinJoyNpadDevice.h>
#include <controller/seadControllerMgr.h>

#include "Library/Controller/NpadController.h"

namespace al {
JoyPadAccelerometerAddon::JoyPadAccelerometerAddon(sead::Controller* controller, s32 index)
    : sead::AccelerometerAddon(controller), mIndex(index) {}

bool JoyPadAccelerometerAddon::calc() {
    sead::ControllerMgr* mgr = sead::ControllerMgr::instance();
    sead::NinJoyNpadDevice* npadDevice = mgr->getControlDeviceAs<sead::NinJoyNpadDevice*>();
    NpadController* npad = static_cast<NpadController*>(mController);

    mIsEnable = false;
    mAcceleration = {0.0f, 0.0, 0.0f};

    if (!npad->isConnected())
        return false;

    s64 index = mIndex;
    if (index >= npad->getSixAxisSensorCount())
        return false;

    const sead::NinJoyNpadDevice::NpadState& npadState =
        npadDevice->getNpadState(npad->getNpadId());
    const nn::hid::SixAxisSensorState& sixAxisState =
        npadState.mSixAxisSensorStates[index].state[0];

    mAcceleration.set(-sixAxisState.mAcceleration[0], sixAxisState.mAcceleration[2],
                      sixAxisState.mAcceleration[1]);

    if (npadDevice->getNpadJoyHoldType() == nn::hid::NpadJoyHoldType::Horizontal) {
        nn::hid::NpadStyleTag style = npad->getStyleIndex();
        if (style == nn::hid::NpadStyleTag::NpadStyleJoyLeft ||
            style == nn::hid::NpadStyleTag::NpadStyleJoyRight) {
            f32 temp = -mAcceleration.x;
            if (style == nn::hid::NpadStyleTag::NpadStyleJoyRight) {
                mAcceleration.x = -mAcceleration.z;
                mAcceleration.z = -temp;
            } else {
                mAcceleration.x = mAcceleration.z;
                mAcceleration.z = temp;
            }
        }
    }

    mIsEnable = true;
    return false;
}

}  // namespace al
