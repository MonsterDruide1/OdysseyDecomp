#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
class CameraTicket;
class LiveActor;
class NerveKeeper;
}  // namespace al

class ShopCamera : public al::IUseExecutor, public al::IUseNerve {
public:
    ShopCamera(const al::LiveActor* actor);

    void execute() override;

    al::NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    void tryStart();
    void tryEnd();
    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeNone();

private:
    const al::LiveActor* mActor = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    sead::Vector3f mCameraPos;
    sead::Vector3f mCameraAt;
    sead::Vector3f mStartCameraPos = sead::Vector3f::zero;
    sead::Vector3f mStartCameraAt = sead::Vector3f::zero;
    al::NerveKeeper* mNerveKeeper = nullptr;
};

static_assert(sizeof(ShopCamera) == 0x58);
