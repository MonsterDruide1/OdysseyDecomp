#include "Player/PlayerInput.h"

#include "Library/Controller/InputFunction.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/Controller/SpinInputAnalyzer.h"
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

void PlayerInput::resetAlongWall() {
    convergeInUpdateWallAlong = 0;
    readInWalls = 0.0f;
}
