

#include "Player/PlayerActionAirMoveControl.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Stuff.h"

PlayerActionAirMoveControl::PlayerActionAirMoveControl(al::LiveActor* actor,
                                                       const PlayerConst* pConst,
                                                       const PlayerInput* input,
                                                       const IUsePlayerCollision* collision,
                                                       bool isDamageOrWallCatch)
    : mLiveActor(actor), mPlayerConst(pConst), mPlayerInput(input), mCollision(collision),
      mIsDamageOrWallCatch(isDamageOrWallCatch) {
    mTurnControl = new PlayerActionTurnControl(actor);
    mTurnControl->_88 = true;
    mTurnControl->_89 = true;
    mTurnControl->setup(pConst->getJumpTurnAngleStart(), pConst->getJumpTurnAngleFast(),
                        pConst->getJumpTurnAngleLimit(), pConst->getJumpTurnAngleFastLimit(),
                        pConst->getJumpTurnAccelFrame(), pConst->getJumpTurnAccelFrameFast(),
                        pConst->getJumpTurnBrakeFrame());
}
void PlayerActionAirMoveControl::setup(float a2, float maxSpeed, int extendFrame, float jumpPower,
                                       float jumpGravity, int a7, float jumpInertia) {
    const PlayerConst* mPlayerConst;        // x0
    al::LiveActor* mLiveActor;              // x1
    IUsePlayerCollision* mCollision;        // x2
    const sead::Vector3f* Velocity;         // x0
    const sead::Vector3f* Gravity;          // x0
    const sead::Vector3f* v19;              // x0
    const sead::Vector3f* v20;              // x0
    float z;                                // s0
    float y;                                // s2
    float x;                                // s1
    float v24;                              // s3
    float v25;                              // s4
    float v26;                              // s6
    float v27;                              // s5
    float v28;                              // s0
    float v29;                              // s0
    float val1;                             // s12
    al::LiveActor* v31;                     // x1
    IUsePlayerCollision* v32;               // x2
    float v33;                              // s0
    al::LiveActor* v34;                     // x21
    float v35;                              // s1
    float v36;                              // s2
    al::LiveActor* v37;                     // x0
    float v38;                              // s3
    float v39;                              // s9
    float v40;                              // s10
    float v41;                              // s11
    const sead::Vector3f* v42;              // x0
    float v43;                              // s1
    float v44;                              // s2
    const PlayerConst* v45;                 // x0
    int v46;                                // w9
    float v47;                              // s0
    PlayerActionTurnControl* mTurnControl;  // x0
    sead::Vector3f v49;                     // [xsp+0h] [xbp-90h] BYREF
    sead::Vector3f v50;                     // [xsp+10h] [xbp-80h] BYREF
    sead::Vector3f v51;                     // [xsp+20h] [xbp-70h] BYREF
    sead::Vector3f a2a;                     // [xsp+30h] [xbp-60h] BYREF
    sead::Vector3f a1;                      // [xsp+40h] [xbp-50h] BYREF

    mPlayerConst = this->mPlayerConst;
    this->mJumpGravity = jumpGravity;
    this->someCounterRunningBackwards = a7;
    this->mMaxFallSpeed = mPlayerConst->getFallSpeedMax();
    a1.x = 0.0;
    a1.y = 0.0;
    a1.z = 0.0;
    a2a.x = 0.0;
    a2a.y = 0.0;
    a2a.z = 0.0;
    mLiveActor = this->mLiveActor;
    mCollision = (IUsePlayerCollision*)this->mCollision;
    if (this->unk)
        rs::calcGroundNormalOrUpDir(&a2a, mLiveActor, mCollision);
    else
        rs::calcGroundNormalOrGravityDir(&a2a, mLiveActor, mCollision);

    Velocity = &al::getVelocity(this->mLiveActor);
    al::verticalizeVec(&a1, a2a, *Velocity);
    Gravity = &al::getGravity(this->mLiveActor);
    al::verticalizeVec(&a1, *Gravity, a1);
    if (!al::tryNormalizeOrZero(&this->anotherVec, a1)) {
        al::calcFrontDir(&this->anotherVec, this->mLiveActor);
        v19 = &al::getGravity(this->mLiveActor);
        al::verticalizeVec(&this->anotherVec, *v19, this->anotherVec);
        if (!al::tryNormalizeOrZero(&this->anotherVec))
            al::calcUpDir(&this->anotherVec, this->mLiveActor);
    }

    v20 = &al::getGravity(this->mLiveActor);
    y = this->anotherVec.y;
    z = this->anotherVec.z;
    x = this->anotherVec.x;
    v24 = v20->x;
    v25 = v20->y;
    v26 = z * v25;
    v27 = v20->z;
    v28 = (float)(z * v20->x) - (float)(x * v27);
    this->someOtherVec.x = (float)(y * v27) - v26;
    this->someOtherVec.y = v28;
    this->someOtherVec.z = (float)(x * v25) - (float)(y * v24);
    al::normalize(&this->someOtherVec);
    v29 = sqrtf((float)((float)(a1.x * a1.x) + (float)(a1.y * a1.y)) + (float)(a1.z * a1.z));
    if (this->unk3) {
        val1 = this->val1;
        if (v29 >= val1) {
            val1 = v29;
            if (v29 > this->mJumpSpeedMax)
                val1 = this->mJumpSpeedMax;
        }
    } else {
        val1 = v29;
    }

    v51.x = 0.0;
    v51.y = 0.0;
    v51.z = 0.0;
    v50.x = 0.0;
    v50.y = 0.0;
    v50.z = 0.0;
    if (val1 >= this->mPlayerConst->getNormalMinSpeed()) {
        v50.x = val1 * this->anotherVec.x;
        v50.y = val1 * this->anotherVec.y;
        v50.z = val1 * this->anotherVec.z;
    }

    v31 = this->mLiveActor;
    v32 = (IUsePlayerCollision*)this->mCollision;
    if (this->unk2)
        rs::calcJumpInertiaWall(&v51, v31, v32, jumpInertia);
    else
        rs::calcJumpInertia(&v51, v31, v32, v50, jumpInertia);

    v33 = sqrtf((float)((float)(v51.x * v51.x) + (float)(v51.y * v51.y)) + (float)(v51.z * v51.z)) +
          maxSpeed;
    v34 = this->mLiveActor;
    v35 = this->anotherVec.x;
    v36 = this->anotherVec.y;
    v37 = this->mLiveActor;
    if (v33 > a2)
        v33 = a2;

    if (val1 <= a2)
        v38 = val1;
    else
        v38 = a2;

    if (v38 >= v33)
        v33 = v38;

    this->somethingHere = v33;
    v39 = v38 * v35;
    v40 = v38 * v36;
    v41 = v38 * this->anotherVec.z;
    v42 = &al::getGravity(v37);
    v43 = v42->y;
    v44 = v42->z;
    v49.x = (float)(v39 - (float)(v42->x * jumpPower)) + v51.x;
    v49.y = (float)(v40 - (float)(v43 * jumpPower)) + v51.y;
    v49.z = (float)(v41 - (float)(v44 * jumpPower)) + v51.z;
    al::setVelocity(v34, v49);
    v45 = this->mPlayerConst;
    if (extendFrame >= 1)
        v46 = extendFrame;
    else
        v46 = 0;

    this->unk_flag2 = extendFrame > 0;
    this->unk_flag3 = 0;
    this->mExtendFrame = v46;
    this->someCounter = 0;
    v47 = v45->getSlerpQuatRate();
    mTurnControl = this->mTurnControl;
    this->something2 = v47;
    this->someVec.x = 0.0;
    this->someVec.y = 0.0;
    this->someVec.z = 0.0;
    mTurnControl->reset();
}
void PlayerActionAirMoveControl::setupTurn(float, float, float, float, int, int, int) {
    CRASH
}
void PlayerActionAirMoveControl::setExtendFrame(int) {
    CRASH
}
void PlayerActionAirMoveControl::setupCollideWallScaleVelocity(float x, float y, float z) {
    isSetCollideWallScaleVelocity = true;
    mCollideWallScaleVelocity = {x, y, z};
}
void PlayerActionAirMoveControl::verticalizeStartMoveDir(const sead::Vector3f&) {
    CRASH
}
void PlayerActionAirMoveControl::update() {
    int mExtendFrame;
    int someCounter;
    bool unk_flag2;
    int v5;
    char isHoldHackJump;
    sead::Vector3f* p_anotherVec;
    PlayerInput* mPlayerInput;
    float* p_y;
    float* p_z;
    float y;
    float z;
    float v15;
    float x;
    float v17;
    float v18;
    const sead::Vector3f* Velocity;
    float v21;
    float v22;
    float v23;
    float v24;
    float v25;
    const PlayerConst** p_mPlayerConst;
    float v27;
    float v28;
    float v29;
    float v30;
    al::LiveActor* mLiveActor;
    float v32;
    float v33;
    float v34;
    al::LiveActor* v35;
    float v36;
    float v37;
    float v38;
    const sead::Vector3f* v39;
    al::LiveActor* v40;
    float v41;
    bool v42;
    PlayerActionTurnControl* mTurnControl;
    const sead::Vector3f* p_someVec;
    al::LiveActor* v45;
    const sead::Vector3f* p__5c;
    float v47;
    PlayerActionTurnControl* v48;
    al::LiveActor* v49;
    int v50;
    sead::Vector3f v51;
    sead::Vector3f v52;
    sead::Vector3f a2;

    if (this->isSetCollideWallScaleVelocity)
        rs::scaleVelocityInertiaWallHit(
            this->mLiveActor, this->mCollision, this->mCollideWallScaleVelocity.x,
            this->mCollideWallScaleVelocity.y, this->mCollideWallScaleVelocity.z);

    mExtendFrame = this->mExtendFrame;
    someCounter = this->someCounter;
    unk_flag2 = this->unk_flag2;
    if (someCounter + 1 <= mExtendFrame + 1)
        v5 = someCounter + 1;
    else
        v5 = mExtendFrame + 1;

    this->someCounter = v5;
    if (unk_flag2 && v5 <= mExtendFrame) {
        if (this->unk_flag3) {
            isHoldHackJump = 1;
        } else {
            if (mIUsePlayerHack)
                isHoldHackJump = rs::isHoldHackJump(this->mIUsePlayerHack);
            else
                isHoldHackJump = mPlayerInput->isHoldJump();
        }

        this->unk_flag2 = isHoldHackJump & 1;
    } else {
        al::addVelocityToGravity(this->mLiveActor, this->mJumpGravity);
    }

    a2 = al::getGravity(this->mLiveActor);
    v52.x = 0.0;
    v52.y = 0.0;
    v52.z = 0.0;
    if (this->someCounterRunningBackwards) {
        p_anotherVec = &this->anotherVec;
    } else {
        v51.x = -a2.x;
        v51.y = -a2.y;
        v51.z = -a2.z;
        if (this->mIUsePlayerHack) {
            CRASH
            // rs::calcHackerMoveVec((__int64)&v52, this->mIUsePlayerHack, &v51);
        } else {
            mPlayerInput = (PlayerInput*)this->mPlayerInput;
            if (this->mIs2D) {
                CRASH
                // PlayerInput::calc2DSnapJumpMoveInput(mPlayerInput, (__int64)&v52, &v51);
            } else
                mPlayerInput->calcMoveInput(&v52, v51);
        }

        p_anotherVec = &this->anotherVec;
        al::verticalizeVec(&this->anotherVec, a2, this->anotherVec);
        if (al::tryNormalizeOrZero(&this->anotherVec)) {
            p_y = &this->anotherVec.y;
            p_z = &this->anotherVec.z;
        } else {
            al::calcFrontDir(&this->anotherVec, this->mLiveActor);
            al::verticalizeVec(&this->anotherVec, a2, this->anotherVec);
            if (!al::tryNormalizeOrZero(&this->anotherVec))
                al::calcUpDir(&this->anotherVec, this->mLiveActor);

            p_y = &this->anotherVec.y;
            y = this->anotherVec.y;
            p_z = &this->anotherVec.z;
            z = this->anotherVec.z;
            v15 = (float)(y * a2.z) - (float)(z * a2.y);
            x = this->anotherVec.x;
            v17 = (float)(z * a2.x) - (float)(x * a2.z);
            v18 = (float)(x * a2.y) - (float)(y * a2.x);
            this->someOtherVec.x = v15;
            this->someOtherVec.y = v17;
            this->someOtherVec.z = v18;
            al::normalize(&this->someOtherVec);
        }

        Velocity = &al::getVelocity(this->mLiveActor);
        v21 = p_anotherVec->x;
        v22 = *p_y;
        v23 = *p_z;
        v24 = (float)((float)(Velocity->x * p_anotherVec->x) + (float)(Velocity->y * *p_y)) +
              (float)(Velocity->z * *p_z);
        if (v24 < 0.0) {
            v21 = -v21;
            v22 = -v22;
            v23 = -v23;
            p_anotherVec->x = v21;
            *p_y = v22;
            *p_z = v23;
        }

        v25 = (float)((float)(v21 * v52.x) + (float)(v22 * v52.y)) + (float)(v23 * v52.z);
        if (v25 > 0.0 || al::isNearZero(v24, 0.001)) {
            p_mPlayerConst = &this->mPlayerConst;
            v27 = this->mPlayerConst->getJumpAccelFront();
        } else {
            p_mPlayerConst = &this->mPlayerConst;
            v27 = this->mPlayerConst->getJumpAccelBack();
        }

        v28 = v27 * (float)(v25 * p_anotherVec->x);
        v29 = v27 * (float)(v25 * *p_y);
        v30 = v27 * (float)(v25 * *p_z);
        mLiveActor = this->mLiveActor;
        v51.x = v28;
        v51.y = v29;
        v51.z = v30;
        al::addVelocity(mLiveActor, v51);
        v32 = v25 * *p_y;
        v33 = v25 * *p_z;
        v34 = v52.x - (float)(v25 * p_anotherVec->x);
        v35 = this->mLiveActor;
        v36 = v52.y - v32;
        v37 = v52.z - v33;
        v38 = (*p_mPlayerConst)->getJumpAccelTurn();
        v51.x = v34 * v38;
        v51.y = v36 * v38;
        v51.z = v37 * v38;
        al::addVelocity(v35, v51);
    }

    al::limitVelocityDir(this->mLiveActor, *p_anotherVec, this->somethingHere);
    al::limitVelocityDir(this->mLiveActor, this->someOtherVec, this->somethingHere);
    v39 = &al::getVelocity(this->mLiveActor);
    if ((float)((float)((float)(v39->x * a2.x) + (float)(v39->y * a2.y)) + (float)(v39->z * a2.z)) >
        0.0)
        al::limitVelocityDir(this->mLiveActor, a2, this->mMaxFallSpeed);

    if (this->mIsDamageOrWallCatch) {
        v40 = this->mLiveActor;
        v41 = this->mPlayerConst->getSlerpQuatRate();
        rs::slerpGravity(v40, v41);
    } else {
        v42 = al::isNearZero(v52, 0.001);
        mTurnControl = this->mTurnControl;
        v51.x = -a2.x;
        v51.y = -a2.y;
        if (v42) {
            p_someVec = &this->someVec;
            v51.z = -a2.z;
        } else {
            v51.z = -a2.z;
            p_someVec = &v52;
        }

        mTurnControl->update(*p_someVec, v51);
        if (this->mIs2D) {
            v45 = this->mLiveActor;
            v51.x = -a2.x;
            v51.y = -a2.y;
            v51.z = -a2.z;
            p__5c = &this->mTurnControl->_5c;
            v47 = this->mPlayerConst->getTall();
            rs::slerpSkyPoseAirSnapSide(v45, v51, *p__5c, v47, this->something2);
        } else {
            v48 = this->mTurnControl;
            v49 = this->mLiveActor;
            if (v48->_6a) {
                v51.z = -a2.z;
                v51.x = -a2.x;
                v51.y = -a2.y;
                rs::slerpSkyPoseAir(v49, v51, v48->_5c, this->something2);
            } else {
                v51.z = -a2.z;
                v51.x = -a2.x;
                v51.y = -a2.y;
                rs::slerpUpFront(v49, v51, v48->_5c, this->something2, 0.0);
            }
        }
    }

    v50 = this->someCounterRunningBackwards - 1;
    if (v50 < 0)
        v50 = 0;

    this->someCounterRunningBackwards = v50;
}
bool PlayerActionAirMoveControl::isHoldJumpExtend() const {
    CRASH
}
void PlayerActionAirMoveControl::calcMoveInput(sead::Vector3f*, const sead::Vector3f&) const {
    CRASH
}
