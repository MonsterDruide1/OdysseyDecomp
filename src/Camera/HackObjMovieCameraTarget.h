#pragma once

#include "Library/Camera/ActorCameraTarget.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Nerve/IUseNerve.h"

class HackObjMovieCameraTarget : public al::ActorCameraTarget,
                                 public al::IUseCamera,
                                 public al::IUseNerve {
public:
    HackObjMovieCameraTarget(al::LiveActor* hackActor);

    const char* getTargetName() const override;
    void calcTrans(sead::Vector3f* trans) const override;
    void updateHack(bool isInHack);
    void changeTargetToHackObj();

    void exeInvalid();
    void exeCenter();
    void exeCenterFix();
    void exeCenterToHackObj();
    void exeCenterFixToHackObj();
    void exeHackObj();

    al::CameraDirector* getCameraDirector() const override;
    al::NerveKeeper* getNerveKeeper() const override;

private:
    al::NerveKeeper* mNerveKeeper = nullptr;
    bool mIsInHack = false;
    s32 mTransitionTime = 0;
    sead::Vector3f _48 = {0, 0, 0};
    sead::Vector3f _54 = {0, 0, 0};
};
