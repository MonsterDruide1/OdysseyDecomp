#pragma once

#include "Library/Camera/ActorCameraTarget.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Nerve/IUseNerve.h"

class HackObjMovieCameraTarget : public al::ActorCameraTarget,
                                 public al::IUseCamera,
                                 public al::IUseNerve {
public:
    HackObjMovieCameraTarget(const al::LiveActor* hackActor);

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

    al::NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

private:
    al::NerveKeeper* mNerveKeeper = nullptr;
    bool mIsInHack = false;
    s32 mTransitionTime = -1;
    sead::Vector3f mFixPlayerPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mFixHackPos = {0.0f, 0.0f, 0.0f};
};
