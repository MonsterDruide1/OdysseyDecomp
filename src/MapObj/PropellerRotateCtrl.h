#pragma once

#include "Library/Joint/JointControllerBase.h"

namespace al {
class LiveActor;
}

class PropellerRotateInfo {
public:
    const char* mJointName;
    s32 mRotateAxis;
    s32 mRotateDirection;
    s32 mRotatePeriod;
};

static_assert(sizeof(PropellerRotateInfo) == 0x18);

class PropellerRotateCtrl : public al::JointControllerBase {
public:
    PropellerRotateCtrl(al::LiveActor* actor, const PropellerRotateInfo& info);

    void update();
    void calcJointCallback(s32 jointIndex, sead::Matrix34f* mtx) override;

    const char* getCtrlTypeName() const override;

private:
    const PropellerRotateInfo* mRotateInfo = nullptr;
    s32 mRotateFrame = 0;
};

static_assert(sizeof(PropellerRotateCtrl) == 0x38);
