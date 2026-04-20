#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

#include "Library/Joint/JointControllerBase.h"

namespace al {
class LiveActor;
}  // namespace al

class CapWorldNpcTailJointController : public al::JointControllerBase {
public:
    CapWorldNpcTailJointController(const al::LiveActor* actor);

    void init();
    void calcJointCallback(s32 jointIndex, sead::Matrix34f* jointMtx) override;
    const char* getCtrlTypeName() const override;

private:
    const al::LiveActor* mActor = nullptr;
    sead::PtrArray<sead::Matrix34f> mTailMatrices;
    sead::PtrArray<s32> mTailJointIndices;
};

static_assert(sizeof(CapWorldNpcTailJointController) == 0x50);

namespace rs {
void initCapWorldNpcTailJointController(al::LiveActor* actor);
}  // namespace rs
