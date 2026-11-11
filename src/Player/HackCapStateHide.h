#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

// Minimal partial reconstruction of the cap's Hide nerve state.
// Only the constructor (signature from data/file_list.yml) and the small
// getters used by HackCap are declared so far; the rest of the class is
// still unknown. Layout from HackCapStateHide.o (ctor 0x40b2d0):
// ActorStateBase (0x00-0x1f), then the seven ctor params at +0x20..+0x50,
// a Vector3f at +0x58 (zeroed by appear, accumulated by
// HackCapFunction::updateSeparateWaitMove), the force-follow flag at +0x64,
// and the separate-hide scale Vector3f at +0x68 (default (1,1,1),
// overridden from the "InitSeparateHideScale" byaml). sizeof 0x74.
namespace al {
class LiveActor;
class PartsModel;
}  // namespace al
class PlayerColliderHackCap;
class PlayerSeparateCapFlag;
class PlayerInput;
class HackCapJointControlKeeper;

class HackCapStateHide : public al::ActorStateBase {
public:
    HackCapStateHide(al::LiveActor*, PlayerColliderHackCap*, const al::LiveActor*,
                     const PlayerSeparateCapFlag*, const PlayerInput*, al::PartsModel*,
                     HackCapJointControlKeeper*);

    void appear() override;
    void kill() override;
    bool update() override;

    bool isSeparateMode() const;
    void cancelSeparateMode();
    void calcSeparateThrowOffset(sead::Vector3f* offset) const;
    void exeHide();
    void exeSeparateWait();
    void tryForceFollowSeparate();

    PlayerColliderHackCap* mCollider;
    const al::LiveActor* mPlayerActor;
    const PlayerSeparateCapFlag* mSeparateCapFlag;
    const PlayerInput* mInput;
    al::PartsModel* mThrowingHatEyes;
    al::PartsModel* mDepthShadow = nullptr;
    HackCapJointControlKeeper* mJointControlKeeper;
    sead::Vector3f _58 = {0.0f, 0.0f, 0.0f};
    // Only the flag at +0x64 is known so far (set by
    // HackCap::requestForceFollowSeparateHide). Name is inferred from that
    // function's name; the rest of the layout is still unknown.
    bool mIsForceFollowSeparateHide = false;
    char _65[3];
    sead::Vector3f mSeparateHideScale = {1.0f, 1.0f, 1.0f};
};
