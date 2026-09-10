#include "Library/Controller/PadGyroAddon.h"

#include <controller/nin/seadNinJoyNpadDevice.h>
#include <controller/seadControllerMgr.h>

#include "Library/Controller/NpadController.h"

namespace al {
PadGyroAddon::PadGyroAddon(sead::Controller* controller, s32 index)
    : sead::ControllerAddon(controller), mIndex(index) {
    mId = sead::ControllerDefine::AddonId::cAddon_Gyro;
}

bool PadGyroAddon::calc() {
    mIsStatusOk = tryUpdateGyroStatus();
    return false;
}

void PadGyroAddon::getPose(sead::Vector3f* outSide, sead::Vector3f* outUp,
                           sead::Vector3f* outFront) const {
    if (outSide)
        outSide->set(mSide);
    if (outUp)
        outUp->set(mUp);
    if (outFront)
        outFront->set(mFront);
}

}  // namespace al
