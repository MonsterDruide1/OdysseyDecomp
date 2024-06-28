#include "Player/PlayerInput.h"

#include "Library/Controller/InputFunction.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/Controller/SpinInputAnalyzer.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"

#include "Library/Player/PlayerHolder.h"
#include "Library/stuff.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInputFunction.h"
#include "Stuff.h"
#include "Util/ActorDimensionKeeper.h"

PlayerInput::PlayerInput(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                         const IUseDimension* dimension)
    : mLiveActor(actor), mPlayerCollision(collision), mDimension(dimension) {
    mSpinInputAnalyzer = new al::SpinInputAnalyzer();
    mJoyPadAccelPoseAnalyzer1 = new al::JoyPadAccelPoseAnalyzer();
    mJoyPadAccelPoseAnalyzer2 = new al::JoyPadAccelPoseAnalyzer();
    mJoyPadAccelPoseAnalyzer1->setSwingBorder(1.3f, 0.2f);
    mJoyPadAccelPoseAnalyzer2->setSwingBorder(1.3f, 0.2f);
    somethingAboutArray = 0;
    arrayOfSizeF0 = new float[60];
    constant60maybe = 60;
}

bool PlayerInput::isEnableCarry() const {
    if (mIsDisableInput)
        return false;

    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryStart() const {
    if (mIsDisableInput)
        return false;

    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryRelease() const {
    if (mIsDisableInput)
        return false;

    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;

    if (mIsDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionMario() const {
    if (mIsDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCarryReleaseBySwing() const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isTriggerAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor))) {
        if (mIsDisableInput)
            return false;
        if (!mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return false;
    }
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerJump() const {
    if (mIsDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension)) {
        if (!mIsDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return true;
    }
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHipDrop() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHeadSliding() const {
    if (mIsDisableInput)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerPaddle() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerRolling(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingRestartSwing() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingCancelHipDrop(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackSwing() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackEnd() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerHackSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSeparateCapJangoHelp() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerJump(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isHoldHackSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isHoldJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerGetOff() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldJump() const {
    if (mIsDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension) && someIntConverged15 > 0)
        return true;
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHipDrop() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartTalk() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerTalk(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartWorldWarp() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerStartWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCancelWorldWarp() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerCancelWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerSpinCap() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerToggleStayCap() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSpinAttackSeparate() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    if (!mIsDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
        return true;
    return PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(0));
}

s32 PlayerInput::getSeparatePlay1P() {
    return al::getPlayerControllerPort(0);
}

bool PlayerInput::isTriggerCapReturn() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor)) {
        if (mIsDisableInput)
            return false;
        auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
        if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
            return true;
        if (mIsDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapAttackSeparate() const {
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionCap() const {
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapSingleHandThrow() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapDoubleHandThrow() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingDoubleHandSameDir();
}

bool PlayerInput::isTriggerCapSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerCapSeparateHipDrop() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSwingPoleClimbFast() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isHoldPoleClimbDown() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerAppendCapAttack(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor) || a1) {
        if (mIsDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isHoldSpinCap() const {
    if (mIsDisableInput || rs::isSeparatePlay(mLiveActor))
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldCapAction() const {
    if (mIsDisableInput)
        return false;
    if (rs::isSeparatePlay(mLiveActor))
        return !PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(1));
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldPoleClimbFast() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldWallCatchMoveFast() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerChange2D() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isTriggerChange3D() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isReleaseJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isEnableDashInput() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isThrowTypeSpiral(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return false;
    f32 absX = (a1.x > 0 ? a1.x : -a1.x);
    f32 absY = (a1.y > 0 ? a1.y : -a1.y);
    return absX > absY;
}

bool PlayerInput::isThrowTypeRolling(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return !al::isNearZero(a1.y, 0.001);
    f32 absX = (a1.x > 0 ? a1.x : -a1.x);
    f32 absY = (a1.y > 0 ? a1.y : -a1.y);
    return !(absX > absY) && !al::isNearZero(a1.y, 0.001);
}

void PlayerInput::update() {
    int convergedInUpdateToDisableInput;          // w0
    bool usedInUpdate;                            // w9
    bool v4;                                      // w8
    const IUsePlayerCollision* mPlayerCollision;  // x2
    bool SnapMoveAreaDir;                         // w8
    float x;                                      // s0
    float y;                                      // s1
    int PlayerInputPort;                          // w20
    int PlayerControllerPort;                     // w0
    int somethingAboutP0;                         // w2
    sead::Vector2f MoveInputStick;                // kr00_8
    float v17;                                    // s1
    float v18;                                    // s0
    float v19;                                    // s1
    int v20;                                      // w20
    const al::IUseSceneObjHolder* v21;            // x0
    int v22;                                      // w0
    int v23;                                      // w2
    sead::Vector2f v24;                           // kr08_8
    float v25;                                    // w8
    float v26;                                    // s1
    float v27;                                    // s0
    int somethingAboutArray;                      // w10
    int v30;                                      // w11
    int constant60maybe_high;                     // w10
    int v32;                                      // w9
    int v33;                                      // w12
    int v34;                                      // w10
    int v35;                                      // w9
    float* arrayOfSizeF0;                         // x10
    int v37;                                      // w9
    float v38;                                    // s0
    int v39;                                      // w8
    int v40;                                      // w10
    int v41;                                      // w11
    int v42;                                      // w12
    int v43;                                      // w11
    bool v46;                                     // w0
    const al::LiveActor* mLiveActor;              // x20
    unsigned int v49;                             // w0
    unsigned int v50;                             // w0
    sead::Vector3f v52;                           // [xsp+18h] [xbp-28h] BYREF
    sead::Vector2f v53;                           // [xsp+28h] [xbp-18h] BYREF

    convergedInUpdateToDisableInput = this->convergedInUpdateToDisableInput;
    usedInUpdate = this->usedInUpdate;
    v4 = convergedInUpdateToDisableInput > 0;
    this->mIsDisableInput = convergedInUpdateToDisableInput > 0;
    if (!usedInUpdate)
        v4 = 0;

    this->usedInUpdate = v4;
    this->convergedInUpdateToDisableInput = al::converge(convergedInUpdateToDisableInput, 0, 1);
    updateWallAlong();
    mPlayerCollision = this->mPlayerCollision;
    SnapMoveAreaDir = 0;
    if (mPlayerCollision)
        SnapMoveAreaDir = rs::tryFindSnapMoveAreaDir(&this->someSnapMoveAreaDirVector,
                                                     this->mLiveActor, mPlayerCollision);

    this->hasSomeSnapMoveDirArea = SnapMoveAreaDir;
    updateInput3D();
    if (this->mIsDisableInput) {
        x = 0.0;
        y = 0.0;
    } else {
        PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        if (rs::isSeparatePlay(mLiveActor)) {
            PlayerControllerPort = al::getPlayerControllerPort(0);
            somethingAboutP0 = this->somethingAboutP0;
            PlayerInputPort = PlayerControllerPort;
        } else {
            somethingAboutP0 = 0;
        }

        MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort,
                                                                somethingAboutP0);
        y = MoveInputStick.y;
        x = MoveInputStick.x;
    }

    v53.x = x;
    v53.y = y;
    if (this->hasSomeSnapMoveDirArea) {
        v52.x = 0.0;
        v52.y = 0.0;
        v52.z = 0.0;
        PlayerInput::calcMoveInputImpl(&v52, -al::getGravity(this->mLiveActor), 0, 0, 0);
        v17 = (float)((float)(v52.x * v52.x) + (float)(v52.y * v52.y)) + (float)(v52.z * v52.z);
    } else {
        v17 = (float)(y * y) + (float)(x * x);
    }

    this->mIsMove = sqrtf(v17) >= 0.1;
    updateInput2D();
    this->mSpinInputAnalyzer->update(v53);
    if (this->mIsDisableInput) {
        v18 = 0.0;
        v19 = 0.0;
    } else {
        v20 = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        if (this->mLiveActor)
            v21 = this->mLiveActor;
        else
            v21 = 0LL;

        if (rs::isSeparatePlay(v21)) {
            v22 = al::getPlayerControllerPort(0);
            v23 = this->somethingAboutP0;
            v20 = v22;
        } else {
            v23 = 0;
        }

        v24 = PlayerInputFunction::getMoveInputStick(this->mLiveActor, v20, v23);
        v19 = v24.y;
        v18 = v24.x;
    }

    sead::Vector2f v52n = {v18, v19};
    if (v52n.length() >= 0.3) {
        sead::Vector2f v51n = {0.0f, 0.0f};
        al::normalize(&v51n, v52n);
        if (al::isNearZero(this->usedInUpdateForAngles, 0.001)) {
            v26 = 0.0;
        } else {
            v27 = al::calcAngleDegree(this->usedInUpdateForAngles, v51n);
            v26 = -45.0;
            if (v27 >= -45.0) {
                v26 = v27;
                if (v27 > 45.0)
                    v26 = 45.0;
            }
        }

        v25 = v26;
        this->usedInUpdateForAngles = v51n;
    } else {
        v25 = 0.0;
    }

    somethingAboutArray = this->somethingAboutArray;
    constant60maybe = this->constant60maybe;
    if (somethingAboutArray < (int)constant60maybe)
        goto LABEL_41;

    v30 = somethingAboutArray - 1;
    if (somethingAboutArray >= 1) {
        constant60maybe_high = (this->constant60maybe2);
        if (constant60maybe_high + 1 < (int)constant60maybe)
            v32 = constant60maybe_high + 1;
        else
            v32 = 0;

        somethingAboutArray = v30;
        (this->constant60maybe2) = v32;
        this->somethingAboutArray = v30;
        constant60maybe = this->constant60maybe;
    }

    if (somethingAboutArray < (int)constant60maybe) {
    LABEL_41:
        v33 = somethingAboutArray + 1;
        v34 = (constant60maybe2) + somethingAboutArray;
        if (v34 < (int)constant60maybe)
            (constant60maybe) = 0;

        v35 = v34 - constant60maybe;
        arrayOfSizeF0 = this->arrayOfSizeF0;
        this->somethingAboutArray = v33;
        arrayOfSizeF0[v35] = v25;
    }

    v37 = this->somethingAboutArray;
    v38 = 0.0;
    v39 = 0;
    this->usedInUpdateForSomething = 0.0;
    v40 = v37 - 1;
    while (v39 < v37) {
        v41 = this->constant60maybe;
        v42 = (this->constant60maybe2);
        if (v40 + v42 < v41)
            v41 = 0;

        v43 = v40 + v42 - v41;
        --v40;
        v38 = this->arrayOfSizeF0[v43] + v38;
        ++v39;
        this->usedInUpdateForSomething = v38;
        if (v39 >= 10)
            goto LABEL_51;
    }

    if (v39 < 1)
        goto LABEL_52;

LABEL_51:
    this->usedInUpdateForSomething = v38 / (float)v39;

LABEL_52:
    al::PadRumbleKeeper* rumbleKeeper = al::getPlayerPadRumbleKeeper(this->mLiveActor, 0LL);
    rumbleKeeper->mPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
    mJoyPadAccelPoseAnalyzer1->update();
    mJoyPadAccelPoseAnalyzer2->update();
    v46 = al::isInAreaObj(this->mLiveActor, "InvalidatePressStickCameraArea");
    mLiveActor = this->mLiveActor;
    this->isInInvalidatePressStickCameraArea = v46;

    if (rs::isSeparatePlay(mLiveActor)) {
        this->somethingAboutP0 = al::converge(this->somethingAboutP0, 0, 1);
        this->somethingAboutP1 = al::converge(this->somethingAboutP1, 0, 1);
        v49 = al::getPlayerControllerPort(0);
        if ((PlayerInputFunction::isTriggerCameraReset(mLiveActor, v49) & 1) != 0)
            this->somethingAboutP0 = 15;

        v50 = al::getPlayerControllerPort(1);
        if ((PlayerInputFunction::isTriggerCameraReset(mLiveActor, v50) & 1) != 0)
            this->somethingAboutP1 = 15;
    } else {
        this->somethingAboutP0 = 0;
        this->somethingAboutP1 = 0;
    }
}

bool PlayerInput::isSpinClockwise() const {
    return mSpinInputAnalyzer->mSpinDirection < 0;
}

void PlayerInput::resetAlongWall() {
    convergeInUpdateWallAlong = 0;
    readInWalls = 0.0f;
}

void sub_710044F630(sead::Vector3f* a1, sead::Vector3f* a2, const sead::Vector3f* a3,
                    const sead::Matrix34f* a4) {
    float v4;   // w23
    float v5;   // w24
    float x;    // s0
    float y;    // s1
    float v9;   // w22
    float z;    // s2
    float v13;  // s5
    float v14;  // s6
    float v15;  // s4
    float v16;  // w25
    float v17;  // w27
    float v18;  // s7
    float v19;  // s16
    float v20;  // s17
    float v21;  // s18
    float v22;  // w26
    float v23;  // w8
    float v24;  // w9
    float v25;  // w10
    float v26;  // s0
    float v27;  // s2
    float v28;  // s1
    float v29;  // s3
    float v30;  // s5
    float v31;  // s6
    float v32;  // s3
    float v33;  // s2
    float v34;  // s5
    float v35;  // s1
    float v36;  // s0
    float v37;  // s1
    float v38;  // s2
    float v39;  // s0

    v4 = a4->m[1][0];
    v5 = a4->m[1][1];
    x = a3->x;
    y = a3->y;
    v9 = a4->m[1][2];
    z = a3->z;
    v14 = a4->m[2][0];
    v13 = a4->m[2][1];
    v15 = a4->m[2][2];
    v16 = a4->m[0][0];
    v17 = a4->m[0][1];
    v18 = (float)((float)(v4 * a3->x) + (float)(v5 * y)) + (float)(v9 * z);
    v19 = (float)((float)(a3->x * (float)-v14) - (float)(y * v13)) - (float)(z * v15);
    v20 = -v19;
    if (v19 > 0.0)
        v20 = (float)((float)(a3->x * (float)-v14) - (float)(y * v13)) - (float)(z * v15);

    v21 = -v18;
    v22 = a4->m[0][2];
    if (v18 > 0.0)
        v21 = (float)((float)(v4 * a3->x) + (float)(v5 * y)) + (float)(v9 * z);

    if (v20 <= v21) {
        if (v18 >= 0.0) {
            v23 = a4->m[2][0];
            v24 = a4->m[2][1];
            v25 = a4->m[2][2];
        } else {
            v25 = -v15;
            v24 = -v13;
            v23 = -v14;
        }
    } else {
        v23 = a4->m[1][0];
        v24 = a4->m[1][1];
        v25 = a4->m[1][2];
        if (v19 < 0.0) {
            v23 = -v4;
            v24 = -v5;
            v25 = -v9;
        }
    }

    a1->x = (float)(y * v25) - (float)(v24 * z);
    a1->y = (float)(v23 * z) - (float)(v25 * x);
    a1->z = (float)(v24 * x) - (float)(v23 * y);
    if (al::tryNormalizeOrZero(a1)) {
        v27 = a1->x;
        v26 = a1->y;
        v28 = a1->z;
    } else {
        v26 = v17;
        v28 = v22;
        a1->x = v16;
        a1->y = v17;
        v27 = v16;
        a1->z = v22;
    }

    v30 = a3->y;
    v29 = a3->z;
    v31 = v26 * v29;
    v32 = v29 * v27;
    v33 = v30 * v27;
    v34 = v28 * v30;
    v35 = (float)(v28 * a3->x) - v32;
    v36 = v33 - (float)(v26 * a3->x);
    a2->x = v31 - v34;
    a2->y = v35;
    a2->z = v36;
    if (al::tryNormalizeOrZero(a2)) {
        v38 = a2->x;
        v37 = a2->y;
        v39 = a2->z;
    } else {
        v39 = v9;
        v37 = v5;
        v38 = v4;
        a2->x = v4;
        a2->y = v5;
        a2->z = v9;
    }

    if ((float)((float)((float)(v4 * v38) + (float)(v5 * v37)) + (float)(v9 * v39)) < 0.0) {
        a2->x = -v38;
        a2->y = -v37;
        a2->z = -v39;
    }
}

void sub_710044FA60(sead::Vector3f* a1, sead::Vector3f* x1_0, const al::LiveActor* a3,
                    const IUseDimension* a4, const sead::Vector2f& a5, const sead::Vector3f& a6,
                    const sead::Vector3f& a7, const sead::Vector3f& a8, const sead::Matrix34f* a9) {
    const sead::Vector3f* Velocity;  // x0
    float v17;                       // s9
    float v18;                       // s10
    float v19;                       // s11
    float v20;                       // s9
    float y;                         // s0
    float x;                         // s1
    float v23;                       // s2
    float v24;                       // s1
    float v25;                       // s0
    float v26;                       // s2
    float v27;                       // s7
    float z;                         // s1
    float v29;                       // s0
    float v30;                       // s1
    float v31;                       // s2
    float v32;                       // s3
    float v33;                       // s9
    float v34;                       // s0
    float v35;                       // s1
    float v36;                       // s2
    float v37;                       // s2
    float v38;                       // s1
    float v39;                       // s0
    sead::Vector3f* v40;             // x0
    float v41;                       // x8^4
    float v42;                       // s2
    sead::Vector3f v43;              // [xsp+0h] [xbp-80h] BYREF
    sead::Vector3f a2;               // [xsp+10h] [xbp-70h] BYREF

    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = 0.0;
    rs::calcLockDirection(&a2, a4);
    v43.x = 0.0;
    v43.y = 0.0;
    v43.z = 0.0;
    Velocity = &al::getVelocity(a3);
    al::verticalizeVec(&v43, a8, *Velocity);
    if (!al::tryNormalizeOrZero(&v43))
        al::calcFrontDir(&v43, a3);

    al::verticalizeVec(&v43, a2, v43);
    if (!al::tryNormalizeOrZero(&v43))
        al::calcFrontDir(&v43, a3);

    v17 = al::sign(a5.x);
    v18 = al::sign(a5.y);
    if (al::isNearZero(v17, 0.001))
        v19 = 1.0;
    else
        v19 = v17;

    if (al::isNearZero(v18, 0.001))
        v18 = 1.0;

    al::verticalizeVec(a1, a2, a6);
    al::verticalizeVec(x1_0, a2, a7);
    if (sqrtf((float)((float)(a1->x * a1->x) + (float)(a1->y * a1->y)) + (float)(a1->z * a1->z)) <
        sqrtf((float)((float)(x1_0->x * x1_0->x) + (float)(x1_0->y * x1_0->y)) +
              (float)(x1_0->z * x1_0->z))) {
        al::normalize(x1_0);
        v20 = (float)((float)(a9->m[0][0] * x1_0->x) + (float)(a9->m[0][1] * x1_0->y)) +
              (float)(a9->m[0][2] * x1_0->z);
        if (al::isNearZero(v20, 0.001)) {
            x = a5.x;
            y = a5.y;
            if (a5.x <= 0.0)
                x = -a5.x;

            v23 = -y;
            if (y > 0.0)
                v23 = a5.y;

            if (x > v23 && !al::isNearZero(y, 0.001)) {
                a1->x = (float)(v19 * v18) * x1_0->x;
                a1->y = (float)(v19 * v18) * x1_0->y;
                a1->z = (float)(v19 * v18) * x1_0->z;
                goto LABEL_44;
            }

            v24 = v19 * v43.y;
            v25 = v19 * v43.x;
            v26 = v19 * v43.z;
        } else {
            if (v20 >= 0.0) {
                a1->z = x1_0->z;
                v41 = x1_0->y;
                a1->x = x1_0->x;
                a1->y = v41;
                goto LABEL_44;
            }

            v24 = -x1_0->y;
            v25 = -x1_0->x;
            v26 = -x1_0->z;
        }

        a1->x = v25;
        a1->y = v24;
        a1->z = v26;

    LABEL_44:
        v40 = a1;
        goto LABEL_45;
    }

    al::normalize(a1);
    v27 = a1->y;
    z = a1->z;
    v29 = (float)((float)(a9->m[1][0] * a1->x) + (float)(a9->m[1][1] * v27)) +
          (float)(a9->m[1][2] * z);
    v30 = (float)((float)-(float)(a9->m[2][1] * v27) - (float)(a9->m[2][0] * a1->x)) -
          (float)(a9->m[2][2] * z);
    v31 = -v29;
    if (v29 > 0.0)
        v31 = v29;

    v32 = -v30;
    if (v30 > 0.0)
        v32 = v30;

    if (v31 >= v32)
        v33 = v29;
    else
        v33 = v30;

    if (al::isNearZero(v33, 0.001)) {
        v34 = a5.x;
        v35 = a5.y;
        if (v35 <= 0.0)
            v35 = -v35;

        v36 = -v34;
        if (v34 > 0.0)
            v36 = a5.x;

        if (v35 > v36 && !al::isNearZero(v34, 0.001)) {
            x1_0->x = (float)(v19 * v18) * a1->x;
            x1_0->y = (float)(v19 * v18) * a1->y;
            v39 = (float)(v19 * v18) * a1->z;
            goto LABEL_42;
        }

        v37 = v43.z;
        v38 = v18 * v43.y;
        x1_0->x = v18 * v43.x;
        v39 = v18 * v37;

    LABEL_41:
        x1_0->y = v38;

    LABEL_42:
        x1_0->z = v39;
        v40 = x1_0;
        goto LABEL_45;
    }

    if (v33 <= 0.0) {
        v42 = a1->z;
        v38 = -a1->y;
        x1_0->x = -a1->x;
        v39 = -v42;
        goto LABEL_41;
    }

    *x1_0 = *a1;
    v40 = x1_0;

LABEL_45:
    al::normalize(v40);
}

bool sub_710044DB24(const sead::Vector3f& a1, const al::LiveActor* a2,
                    const IUsePlayerCollision* a3) {
    float v7;
    sead::Vector3f v8;

    v8.x = 0.0;
    v8.y = 0.0;
    v8.z = 0.0;
    al::calcUpDir(&v8, a2);
    al::normalize(&v8);
    if ((float)((float)((float)(a1.x * v8.x) + (float)(a1.y * v8.y)) + (float)(a1.z * v8.z)) <
        -0.86603)
        return 1LL;

    v8.x = 0.0;
    v8.y = 0.0;
    v8.z = 0.0;
    al::calcUpDir(&v8, a2);
    al::normalize(&v8);
    v7 = (float)((float)(a1.x * v8.x) + (float)(a1.y * v8.y)) + (float)(a1.z * v8.z);
    if (rs::isCollidedGround(a3))
        return v7 >= 0.98481;
    else
        return v7 >= 0.86603;
}

void PlayerInput::updateInput2D() {
    const IUseDimension* mDimension;
    int* p_someIntConverged15;
    const IUseDimension* v4;
    bool v5;
    float x;
    float y;
    int PlayerInputPort;
    const al::IUseSceneObjHolder* v9;
    int PlayerControllerPort;
    int somethingAboutP0;
    sead::Vector2f MoveInputStick;
    float v13;
    float v14;
    float z;
    float v16;
    float v17;
    float v18;
    float v19;
    float v20;
    float v21;
    float v22;
    float v23;
    const sead::Vector3f* CollidedGroundPos;
    float v25;
    const IUsePlayerCollision* mPlayerCollision;
    bool v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    bool v34;
    bool isCollidedGround;
    bool v36;
    const sead::Vector3f* Trans;
    float v38;
    float v39;
    bool v40;
    const sead::Matrix34f* PlayerViewMtx;
    const sead::Matrix34f* v42;
    float v43;
    float v44;
    float v45;
    float v46;
    float v47;
    float v48;
    const IUseDimension* v49;
    float v50;
    float v51;
    float v52;
    float v53;
    float v54;
    float v55;
    const sead::Vector3f* v56;
    float v57;
    const IUsePlayerCollision* v58;
    const sead::Vector3f* CollidedGroundNormal;
    float v60;
    sead::Vector3f a3;
    sead::Vector3f v62;
    sead::Vector3f v63;
    sead::Vector3f a1;
    sead::Vector3f a2;
    float v66;
    float v67;

    if (!this->mPlayerCollision)
        return;

    mDimension = this->mDimension;
    if (!mDimension)
        return;

    if ((rs::is2D(this->mDimension) & 1) != 0 && (rs::isIn2DArea(mDimension) & 1) != 0) {
        if (this->mIsDisableInput) {
            p_someIntConverged15 = &this->someIntConverged15;

        LABEL_10:
            *p_someIntConverged15 = al::converge(*p_someIntConverged15, 0, 1);
            goto LABEL_11;
        }

        p_someIntConverged15 = &this->someIntConverged15;
        if (!mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            goto LABEL_10;

        *p_someIntConverged15 = 15;
    } else {
        this->someIntConverged15 = 0;
    }

LABEL_11:
    v4 = this->mDimension;
    if (!v4 || (rs::is2D(this->mDimension) & 1) == 0 || (rs::isIn2DArea(v4) & 1) == 0 ||
        this->someFlags[0]) {
        v5 = this->someFlags[1];
        this->someFlags[0] = 0;
        if (v5) {
            this->someFlags[1] = 0;
            memset(&this->someVector2, 0, 0x70uLL);
        }

        return;
    }

    if (this->mIsDisableInput) {
        x = 0.0;
        y = 0.0;
    } else {
        PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        v9 = this->mLiveActor;

        if (rs::isSeparatePlay(v9)) {
            PlayerControllerPort = al::getPlayerControllerPort(0);
            somethingAboutP0 = this->somethingAboutP0;
            PlayerInputPort = PlayerControllerPort;
        } else {
            somethingAboutP0 = 0;
        }

        MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort,
                                                                somethingAboutP0);
        y = MoveInputStick.y;
        x = MoveInputStick.x;
    }

    if (!isSameStickMove(this->someVector2, 10.0))
        goto LABEL_44;

    v13 = this->someVectorCollidedGroundPos.x;
    v66 = this->someVectorCollidedGroundPos.y;
    v14 = sqrtf((float)(this->some2Vector.x * this->some2Vector.x) +
                (float)(this->some2Vector.y * this->some2Vector.y));
    z = this->someVectorCollidedGroundPos.z;
    v16 = this->someVectorCollidedGroundNormal.x;
    v17 = this->someVectorCollidedGroundNormal.y;
    v18 = this->someVectorCollidedGroundNormal.z;
    v67 = x;
    v19 = sqrtf((float)(x * x) + (float)(y * y));
    if (v14 < v19) {
        v20 = sqrtf((float)(this->some2Vector.x * this->some2Vector.x) +
                    (float)(this->some2Vector.y * this->some2Vector.y));
        if (v20 > 0.0) {
            v21 = v19 / v20;
            v22 = v21 * this->some2Vector.x;
            v23 = v21 * this->some2Vector.y;
            this->some2Vector.x = v22;
            this->some2Vector.y = v23;
        }
    }

    if (rs::isCollidedGround(this->mPlayerCollision)) {
        CollidedGroundPos = &rs::getCollidedGroundPos(this->mPlayerCollision);
        this->someVectorCollidedGroundPos.x = CollidedGroundPos->x;
        this->someVectorCollidedGroundPos.y = CollidedGroundPos->y;
        v25 = CollidedGroundPos->z;
        mPlayerCollision = this->mPlayerCollision;
        this->someVectorCollidedGroundPos.z = v25;
        this->someVectorCollidedGroundNormal = rs::getCollidedGroundNormal(mPlayerCollision);
    }

    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = 0.0;
    rs::calcGroundNormalOrGravityDir(&a2, this->mLiveActor, this->mPlayerCollision);
    a1.x = 0.0;
    a1.y = 0.0;
    a1.z = 0.0;
    calcMoveInputImpl(&a1, a2, 0, 0, 0);
    v27 = al::tryNormalizeOrZero(&a1);
    if (rs::isJustLand(this->mPlayerCollision)) {
        v28 = (float)(v16 * (float)(this->someVectorCollidedGroundPos.x - v13)) +
              (float)(v17 * (float)(this->someVectorCollidedGroundPos.y - v66));
        v29 = v18 * (float)(this->someVectorCollidedGroundPos.z - z);
        v30 = this->someVectorCollidedGroundNormal.x;
        v31 = v18;
        v32 = this->someVectorCollidedGroundNormal.y;
        v33 = this->someVectorCollidedGroundNormal.z;
        v34 = al::isNearZeroOrLess(v28 + v29, 0.001) ||
              al::isNearZeroOrGreater(
                  (float)((float)(v16 * v30) + (float)(v17 * v32)) + (float)(v31 * v33), 0.001);
        x = v67;
        if (v27 && v34)
            return;

        v36 = v34 || !v27;
    } else {
        isCollidedGround = rs::isCollidedGround(this->mPlayerCollision);
        x = v67;
        if (isCollidedGround) {
            if (v27)
                return;

            goto LABEL_44;
        }

        Trans = &al::getTrans(this->mLiveActor);
        v38 = (float)((float)(v16 * (float)(Trans->x - v13)) +
                      (float)(v17 * (float)(Trans->y - v66))) +
              (float)(v18 * (float)(Trans->z - z));
        v39 = (float)((float)(v16 * a2.x) + (float)(v17 * a2.y)) + (float)(v18 * a2.z);
        v40 = al::isNearZeroOrLess(v38, 0.001) || v39 >= -0.70711;
        if (v27 && v40)
            return;

        v36 = v40 || !v27;
    }

    if (v36) {
    LABEL_44:
        a1.x = 0.0;
        a1.y = 0.0;
        a1.z = 0.0;
        if (!this->mIsDisableInput) {
            a2.x = 0.0;
            a2.y = 0.0;
            a2.z = 0.0;
            al::calcSideDir(&a2, this->mLiveActor);
            al::normalize(&a2);
            calcMoveInputImpl(&a1, a2, 0, 1, 0);
            al::tryNormalizeOrZero(&a1);
        }

        this->mIsMove &= !sub_710044DB24(a1, this->mLiveActor, this->mPlayerCollision);
    }

    if (this->mIsDisableInput || !this->mIsMove) {
        this->someFlags[1] = 0;
        memset(&this->someVector2, 0, 0x70uLL);
    } else {
        PlayerViewMtx = PlayerFunction::getPlayerViewMtx(this->mLiveActor);
        this->someFlags[1] = 1;
        this->someVector2.x = x;
        this->someVector2.y = y;
        a1.x = 0.0;
        a1.y = 0.0;
        v42 = PlayerViewMtx;
        a1.z = 0.0;
        al::calcSideDir(&a1, this->mLiveActor);
        al::normalize(&a1);
        v63.x = 0.0;
        v63.y = 0.0;
        v63.z = 0.0;
        v62.x = 0.0;
        v62.y = 0.0;
        v62.z = 0.0;
        sub_710044F630(&v63, &v62, &a1, v42);
        a3.x = 0.0;
        a3.y = 0.0;
        a3.z = 0.0;
        if (!this->mIsDisableInput) {
            a2.x = 0.0;
            a2.y = 0.0;
            a2.z = 0.0;
            al::calcSideDir(&a2, this->mLiveActor);
            al::normalize(&a2);
            calcMoveInputImpl(&a3, a2, 0, 1, 0);
            al::tryNormalizeOrZero(&a3);
        }

        a2.x = 0.0;
        a2.y = 0.0;
        a2.z = 0.0;
        al::calcUpDir(&a2, this->mLiveActor);
        al::normalize(&a2);
        al::verticalizeVec(&a3, a2, a3);
        v43 = (float)((float)(a3.x * v62.x) + (float)(a3.y * v62.y)) + (float)(a3.z * v62.z);
        this->some2Vector.x =
            (float)((float)(v63.x * a3.x) + (float)(v63.y * a3.y)) + (float)(v63.z * a3.z);
        this->some2Vector.y = v43;
        v44 = sqrtf((float)(x * x) + (float)(y * y));
        v45 = sqrtf((float)(this->some2Vector.x * this->some2Vector.x) +
                    (float)(this->some2Vector.y * this->some2Vector.y));
        if (v45 > 0.0) {
            v46 = v44 / v45;
            v47 = v46 * this->some2Vector.x;
            v48 = v46 * this->some2Vector.y;
            this->some2Vector.x = v47;
            this->some2Vector.y = v48;
        }

        a1.x = 0.0;
        a1.y = 0.0;
        a1.z = 0.0;
        rs::calcGroundNormalOrGravityDir(&a1, this->mLiveActor, this->mPlayerCollision);
        sub_710044F630(&this->_E0, &this->_EC, &a1, v42);
        this->someVector = a1;
        al::normalize(&this->someVector);
        sub_710044FA60(&this->someVector4, &this->someVector5, this->mLiveActor, this->mDimension,
                       this->some2Vector, this->_E0, this->_EC, a1, v42);
        v49 = this->mDimension;
        v50 = v42->m[2][0];
        v51 = v42->m[2][1];
        v52 = v42->m[2][2];
        a2.x = 0.0;
        a2.y = 0.0;
        a2.z = 0.0;
        rs::calcLockDirection(&a2, v49);
        v53 = (float)((float)-(float)(v51 * a2.y) - (float)(v50 * a2.x)) - (float)(v52 * a2.z);
        if (al::isNearZero(v53, 0.001) || v53 > 0.0) {
            v54 = a2.z;
            v55 = -a2.x;
            this->someVector3.y = -a2.y;
            this->someVector3.x = v55;
            this->someVector3.z = -v54;
        } else {
            this->someVector3 = a2;
        }

        if (rs::isCollidedGround(this->mPlayerCollision)) {
            v56 = &rs::getCollidedGroundPos(this->mPlayerCollision);
            this->someVectorCollidedGroundPos.x = v56->x;
            this->someVectorCollidedGroundPos.y = v56->y;
            v57 = v56->z;
            v58 = this->mPlayerCollision;
            this->someVectorCollidedGroundPos.z = v57;
            CollidedGroundNormal = &rs::getCollidedGroundNormal(v58);
            this->someVectorCollidedGroundNormal.x = CollidedGroundNormal->x;
            this->someVectorCollidedGroundNormal.y = CollidedGroundNormal->y;
            v60 = CollidedGroundNormal->z;
        } else {
            this->someVectorCollidedGroundPos = al::getTrans(this->mLiveActor);
            this->someVectorCollidedGroundNormal.x = a1.x;
            this->someVectorCollidedGroundNormal.y = a1.y;
            v60 = a1.z;
        }

        this->someVectorCollidedGroundNormal.z = v60;
    }
}

bool PlayerInput::isSameStickMove(const sead::Vector2f& a2, float a3) const {
    float x;                           // s0
    float y;                           // s1
    int PlayerInputPort;               // w20
    const al::IUseSceneObjHolder* v8;  // x0
    int PlayerControllerPort;          // w0
    int somethingAboutP0;              // w2
    sead::Vector2f MoveInputStick;     // kr00_8
    float v12;                         // s9
    sead::Vector2f v14;                // [xsp+0h] [xbp-30h] BYREF
    sead::Vector2f v15;                // [xsp+8h] [xbp-28h] BYREF

    if (this->mIsDisableInput)
        return 0LL;

    if (!this->mIsMove)
        return 0LL;

    v15 = a2;
    if ((al::tryNormalizeOrZero(&v15) & 1) == 0)
        return 0LL;

    if (this->mIsDisableInput) {
        x = 0.0;
        y = 0.0;
    } else {
        PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        v8 = this->mLiveActor;
        if (rs::isSeparatePlay(v8)) {
            PlayerControllerPort = al::getPlayerControllerPort(0);
            somethingAboutP0 = this->somethingAboutP0;
            PlayerInputPort = PlayerControllerPort;
        } else {
            somethingAboutP0 = 0;
        }

        MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort,
                                                                somethingAboutP0);
        y = MoveInputStick.y;
        x = MoveInputStick.x;
    }

    v14.x = x;
    v14.y = y;
    if ((al::tryNormalizeOrZero(&v14) & 1) == 0)
        return 0LL;

    v12 = (float)(v15.x * v14.x) + (float)(v15.y * v14.y);
    return v12 >= sead::Mathf::cos(a3 * 0.017453);
}

void sub_710044F864(sead::Vector3f* a1, sead::Vector3f* x1_0, const sead::Vector3f& x2_0,
                    const sead::Matrix34f* a4, float a5, float a6, float a7) {
    float v7;            // w12
    float v8;            // w13
    float v9;            // w8
    float v10;           // w10
    float v11;           // s6
    float v12;           // s3
    float v13;           // s4
    float v14;           // s17
    float v15;           // s5
    float v16;           // s18
    float v17;           // s0
    float x;             // s1
    float v19;           // s18
    float v20;           // s19
    float v21;           // s20
    float v22;           // s21
    float y;             // s1
    float z;             // s3
    float v28;           // s2
    float v29;           // s5
    float v30;           // s6
    float v31;           // s2
    float v32;           // s0
    sead::Vector3f v34;  // [xsp+0h] [xbp-50h] BYREF
    sead::Vector3f a3;   // [xsp+10h] [xbp-40h] BYREF
    sead::Vector3f a2;   // [xsp+20h] [xbp-30h] BYREF

    v7 = a4->m[0][1];
    v8 = a4->m[0][2];
    a2.x = a4->m[0][0];
    a2.y = v7;
    a2.z = v8;
    v9 = a4->m[1][2];
    v10 = a4->m[1][1];
    v11 = a4->m[1][0];
    v13 = a4->m[2][0];
    v12 = a4->m[2][1];
    v14 = -v13;
    v15 = a4->m[2][2];
    v16 = (float)(v11 * a5) + (float)(v10 * a6);
    v17 = (float)(a5 * (float)-v13) - (float)(a6 * v12);
    x = a2.x;
    v19 = v16 + (float)(v9 * a7);
    v20 = v17 - (float)(a7 * v15);
    if (v20 <= 0.0)
        v21 = -v20;
    else
        v21 = v17 - (float)(a7 * v15);

    if (v19 <= 0.0)
        v22 = -v19;
    else
        v22 = v19;

    if (v21 > v22) {
        if (v20 < 0.0) {
            x1_0->x = a4->m[1][0];
            x1_0->y = v10;
            v13 = v11;
            v12 = v10;
            v15 = v9;
            x1_0->z = v9;
            goto LABEL_15;
        }

        v13 = -v11;
        v12 = -v10;
        v15 = -v9;
        goto LABEL_13;
    }

    if (v19 < 0.0) {
    LABEL_13:
        x1_0->x = v13;
        x1_0->y = v12;
        goto LABEL_14;
    }

    v12 = -v12;
    v15 = -v15;
    v13 = -v13;
    x1_0->x = v14;
    x1_0->y = v12;

LABEL_14:
    x1_0->z = v15;

LABEL_15:
    a3.x = (float)(v7 * v15) - (float)(v8 * v12);
    a3.y = (float)(v8 * v13) - (float)(v15 * x);
    a3.z = (float)(v12 * x) - (float)(v7 * v13);
    al::normalize(&a3);
    v34.x = 0.0;
    v34.y = 0.0;
    v34.z = 0.0;
    al::verticalizeVec(&v34, a2, x2_0);
    if (!al::tryNormalizeOrZero(&v34))
        v34 = x2_0;

    al::alongVectorNormalH(x1_0, *x1_0, a3, v34);
    al::normalize(x1_0);
    y = x1_0->y;
    z = x1_0->z;
    v29 = x2_0.y;
    v28 = x2_0.z;
    v30 = y * v28;
    v31 = (float)(z * x2_0.x) - (float)(v28 * x1_0->x);
    v32 = (float)(v29 * x1_0->x) - (float)(y * x2_0.x);
    a1->x = v30 - (float)(z * v29);
    a1->y = v31;
    a1->z = v32;
    al::normalize(a1);
}

void PlayerInput::updateInput3D() {
    const IUseDimension* mDimension;
    bool somethingFromUpdate;
    float x;
    float y;
    int PlayerInputPort;
    const al::IUseSceneObjHolder* v7;
    int PlayerControllerPort;
    int somethingAboutP0;
    sead::Vector2f MoveInputStick;
    float v11;
    float v12;
    float v13;
    float v14;
    float v15;
    const al::IUseCamera* v16;
    const sead::Matrix34f* PlayerViewMtx;
    const sead::Matrix34f* v18;
    const sead::Vector3f* Gravity;
    sead::Vector3f v20;

    if (!this->mPlayerCollision)
        return;

    mDimension = this->mDimension;
    if ((mDimension && (rs::is2D(this->mDimension) & 1) != 0 &&
         (rs::isIn2DArea(mDimension) & 1) != 0) ||
        this->flag2) {
        this->flag2 = 0;
        goto LABEL_7;
    }

    if (!al::isInAreaObj((al::LiveActor*)this->mLiveActor, "MoveDirectionKeepArea")) {
    LABEL_7:
        if (this->somethingFromUpdate) {
            this->somethingFromUpdate = 0;
            this->someVectorEndOfUpdate3.z = 0.0;
            this->someVectorUsedInUpdate.x = 0.0;
            this->someVectorUsedInUpdate.y = 0.0;
            this->someVectorEndOfUpdate2.y = 0.0;
            this->someVectorEndOfUpdate2.z = 0.0;
            this->someVectorEndOfUpdate3.x = 0.0;
            this->someVectorEndOfUpdate3.y = 0.0;
            this->someVectorEndOfUpdate.x = 0.0;
            this->someVectorEndOfUpdate.y = 0.0;
            this->someVectorEndOfUpdate.z = 0.0;
            this->someVectorEndOfUpdate2.x = 0.0;
        }

        return;
    }

    if (this->mIsDisableInput) {
        somethingFromUpdate = this->somethingFromUpdate;
        x = 0.0;
        y = 0.0;
    } else {
        PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        v7 = this->mLiveActor;

        if (rs::isSeparatePlay(v7)) {
            PlayerControllerPort = al::getPlayerControllerPort(0);
            somethingAboutP0 = this->somethingAboutP0;
            PlayerInputPort = PlayerControllerPort;
        } else {
            somethingAboutP0 = 0;
        }

        MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort,
                                                                somethingAboutP0);
        somethingFromUpdate = this->somethingFromUpdate;
        y = MoveInputStick.y;
        x = MoveInputStick.x;
    }

    if (somethingFromUpdate) {
        if (isSameStickMove(this->someVectorUsedInUpdate, 10.0)) {
            v11 = sqrtf((float)(x * x) + (float)(y * y));
            if (sqrtf((float)(this->someVectorUsedInUpdate.x * this->someVectorUsedInUpdate.x) +
                      (float)(this->someVectorUsedInUpdate.y * this->someVectorUsedInUpdate.y)) <
                v11) {
                v12 =
                    sqrtf((float)(this->someVectorUsedInUpdate.x * this->someVectorUsedInUpdate.x) +
                          (float)(this->someVectorUsedInUpdate.y * this->someVectorUsedInUpdate.y));
                if (v12 > 0.0) {
                    v13 = v11 / v12;
                    v14 = v13 * this->someVectorUsedInUpdate.x;
                    v15 = v13 * this->someVectorUsedInUpdate.y;
                    this->someVectorUsedInUpdate.x = v14;
                    this->someVectorUsedInUpdate.y = v15;
                }
            }

            return;
        }

        this->somethingFromUpdate = 0;
        this->someVectorEndOfUpdate3.z = 0.0;
        this->someVectorUsedInUpdate.x = 0.0;
        this->someVectorUsedInUpdate.y = 0.0;
        this->someVectorEndOfUpdate2.y = 0.0;
        this->someVectorEndOfUpdate2.z = 0.0;
        this->someVectorEndOfUpdate3.x = 0.0;
        this->someVectorEndOfUpdate3.y = 0.0;
        this->someVectorEndOfUpdate.x = 0.0;
        this->someVectorEndOfUpdate.y = 0.0;
        this->someVectorEndOfUpdate.z = 0.0;
        this->someVectorEndOfUpdate2.x = 0.0;
    }

    v16 = this->mLiveActor;

    if ((al::isActiveCameraInterpole(v16, 0) & 1) != 0) {
        PlayerViewMtx = PlayerFunction::getPlayerViewMtx(this->mLiveActor);
        this->somethingFromUpdate = 1;
        this->someVectorUsedInUpdate.x = x;
        this->someVectorUsedInUpdate.y = y;
        v18 = PlayerViewMtx;
        v20.x = 0.0;
        v20.y = 0.0;
        v20.z = 0.0;
        rs::calcGroundNormalOrGravityDir(&v20, this->mLiveActor, this->mPlayerCollision);
        Gravity = &al::getGravity(this->mLiveActor);
        sub_710044F864(&this->someVectorEndOfUpdate, &this->someVectorEndOfUpdate2, v20, v18,
                       -Gravity->x, -Gravity->y, -Gravity->z);
        this->someVectorEndOfUpdate3 = v20;
        al::normalize(&this->someVectorEndOfUpdate3);
    }
}

void PlayerInput::updateWallAlong()
{
  float x; // s0
  float y; // s1
  float v5; // t1
  float v6; // s0
  float v7; // s0
  int PlayerInputPort; // w20
  const al::IUseSceneObjHolder *v9; // x0
  int PlayerControllerPort; // w0
  int somethingAboutP0; // w2
  sead::Vector2f MoveInputStick; // kr00_8
  float v13; // w20
  float v14; // s0
  float v15; // w8
  float v16; // w9
  float readInWalls; // t1
  sead::Vector3f v18; // [xsp+0h] [xbp-30h] BYREF

  if ( this->mPlayerCollision )
  {
    this->convergeInUpdateWallAlong = al::converge(this->convergeInUpdateWallAlong, 0, 1);
    v18.x = 0.0;
    v18.y = 0.0;
    v18.z = 0.0;
    if ( rs::tryGetPlayerInputWallAlongWallNormal(&v18, this->mLiveActor) )
    {
      if ( this->mIsDisableInput )
      {
        x = 0.0;
        y = 0.0;
      }
      else
      {
        PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
        v9 = this->mLiveActor;

        if ( rs::isSeparatePlay(v9) )
        {
          PlayerControllerPort = al::getPlayerControllerPort(0);
          somethingAboutP0 = this->somethingAboutP0;
          PlayerInputPort = PlayerControllerPort;
        }
        else
        {
          somethingAboutP0 = 0;
        }

        MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort, somethingAboutP0);
        y = MoveInputStick.y;
        x = MoveInputStick.x;
      }

      v13 = x;
      v14 = sqrtf((float)(x * x) + (float)(y * y));
      if ( v14 >= 0.1 )
        v15 = y;
      else
        v15 = 0.0;

      if ( v14 >= 0.1 )
        v16 = v13;
      else
        v16 = 0.0;

      if ( !this->convergeInUpdateWallAlong
        || (float)((float)((float)(v18.x * this->anotherVectorForWalls.x)
                         + (float)(v18.y * this->anotherVectorForWalls.y))
                 + (float)(v18.z * this->anotherVectorForWalls.z)) > 0.087156
        || (float)((float)(v16 * this->anotherVector2ForWalls2.x) + (float)(v15 * this->anotherVector2ForWalls2.y)) < 0.98481 )
      {
        this->anotherVectorForWalls = v18;
        this->anotherVector2ForWalls2.x = v16;
        this->anotherVector2ForWalls2.y = v15;
      }

      this->convergeInUpdateWallAlong = 5;
      readInWalls = this->readInWalls;
      v6 = readInWalls;
      goto LABEL_26;
    }

    v5 = this->readInWalls;
    v6 = v5;
    if ( this->convergeInUpdateWallAlong >= 1 )
    {
LABEL_26:
      this->readInWalls = v6 + (float)((float)(1.0 - v6) * 0.1);
      return;
    }

    v7 = v6 + (float)((float)(0.0 - v6) * 0.05);
    this->readInWalls = v7;
    if ( v7 < 0.005 )
      this->readInWalls = 0.0;
  }
}
