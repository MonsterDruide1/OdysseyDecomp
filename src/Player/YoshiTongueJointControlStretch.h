#pragma once

#include <math/seadBoundBox.h>

#include "Library/Joint/JointControllerBase.h"

namespace al {
class LiveActor;
}

class YoshiTongueJointControlStretch : al::JointControllerBase {
public:
    YoshiTongueJointControlStretch(const al::LiveActor* tongueActor);

    void calcJointCallback(s32, sead::Matrix34f* outMtx) override;
    void update(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);

    void calcBoundingBox(sead::BoundBox3f* outBox) const;
    const char* getCtrlTypeName() const override;  // 伸縮

private:
    const al::LiveActor* mTongueActor = nullptr;
    char mPadding[0x68 - sizeof(al::JointControllerBase)];
};

static_assert(sizeof(YoshiTongueJointControlStretch) == 0x70);
