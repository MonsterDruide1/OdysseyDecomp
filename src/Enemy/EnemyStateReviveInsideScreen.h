#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class EnemyStateReviveInsideScreen : public al::ActorStateBase {
public:
    EnemyStateReviveInsideScreen(al::LiveActor*);

    void appear() override;
    void kill() override;

    void startRevive();
    bool isDisappear() const;

    void exeHide();
    void exeDelay();
    void exeAppearSign();

private:
    sead::Quatf mReviveQuat = sead::Quatf::unit;
    sead::Vector3f mRevivePos = sead::Vector3f::zero;
    bool mIsHidden = false;
    bool mIsModelHidden = false;
    bool mIsInvalidClipping = false;
};
