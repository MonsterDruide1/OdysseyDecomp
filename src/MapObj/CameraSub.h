#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class AreaObj;
class CameraTicket;
class SubCameraRenderer;
}  // namespace al

class CameraSub : public al::LiveActor {
public:
    CameraSub(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void onAppear();
    void initAfterPlacement() override;
    void exeWait();
    void exeActive();

private:
    al::CameraTicket* mCameraTicket = nullptr;
    al::AreaObj* mAreaObj = nullptr;
    al::SubCameraRenderer* mSubCameraRenderer = nullptr;
    sead::Vector3f mScreenScale = sead::Vector3f::zero;
    sead::Matrix34f mScreenMatrix = sead::Matrix34f::ident;
    bool mHasScreenInfo = false;
};

static_assert(sizeof(CameraSub) == 0x160);
