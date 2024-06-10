#include "Player/PlayerActionGroundMoveControl.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "Player/PlayerActionTurnControl.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/Hack.h"
#include "Util/StageSceneFunction.h"
#include "math/seadVectorFwd.h"

PlayerActionGroundMoveControl::PlayerActionGroundMoveControl(al::LiveActor* player,
                                                             PlayerConst const* pConst,
                                                             PlayerInput const* input,
                                                             IUsePlayerCollision const* collision)
    : mActor(player), mConst(pConst), mInput(input), mCollision(collision),
      mTurnControl(new PlayerActionTurnControl(player)) {}
void PlayerActionGroundMoveControl::initDash(IJudge*, float, int) {
    CRASH
}
void PlayerActionGroundMoveControl::setupDash(float, int) {
    CRASH
}
void PlayerActionGroundMoveControl::setup(float a2, float a3, int a4, int a5, int a6, float a7,
                                          float a8, int a9) {
    someFlag = 1;
    mMaxSpeed = a2;
    mMinSpeed = a3;
    mAccelFrame = a4;
    mStickOnBrakeFrame = a5;
    mNormalBrakeFrame = a6;
    mGravity = a7;
    _80 = 0;
    mBrakeOnSpeed = a8;
    mBrakeOnCounterCorner = a9;
}
void PlayerActionGroundMoveControl::appear() {
    sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalExceptJustLandOrGravityDir(&a3, mActor, mCollision);
    rs::calcGroundNormalOrGravityDir(&mGravityDir, mActor, mCollision);
    someVec = mGravityDir;
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    al::alongVectorNormalH(&a1, al::getVelocity(mActor), a3, mGravityDir);
    sead::Vector3f v12 = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&v12, mActor);
    al::verticalizeVec(&v12, mGravityDir, v12);
    al::tryNormalizeOrZero(&v12);
    al::setVelocity(mActor, a1 - (mGravity * mGravityDir));
    someVec = mGravityDir;
    _7C = 0.0;
    calcInitBrakeOnCounter();
    mTurnControl->reset();
    _C0 = 0;
}
void PlayerActionGroundMoveControl::reset(sead::Vector3f const& a2) {
    mGravityDir = a2;
    someVec = a2;
    _7C = 0.0;
    calcInitBrakeOnCounter();
    mTurnControl->reset();
    _C0 = 0;
}
void PlayerActionGroundMoveControl::calcInitBrakeOnCounter() {
    float v2;           // s8
    sead::Vector3f v3;  // [xsp+0h] [xbp-40h] BYREF
    sead::Vector3f a1;  // [xsp+10h] [xbp-30h] BYREF

    this->_60 = 0;
    updateNormalAndSnap(&a1);
    v2 = a1.length();
    if ((rs::isOnGroundSkateCode(this->mActor, this->mCollision) && !al::isNearZero(v2, 0.001)) ||
        (v2 >= this->mBrakeOnSpeed && al::tryNormalizeOrZero(&a1) &&
         (al::calcFrontDir(&v3, this->mActor), al::verticalizeVec(&v3, this->mGravityDir, v3),
          al::tryNormalizeOrZero(&v3)) &&
         !PlayerActionFunction::isOppositeDir(v3, a1))) {
        this->_60 = this->mBrakeOnCounterCorner;
    }
}
void PlayerActionGroundMoveControl::update() {
    if (rs::isOnGroundSkateCode(mActor, mCollision))
        updateSkateMove();
    else
        updateNormalMove();
}
void PlayerActionGroundMoveControl::updateSkateMove() {
    CRASH
}

void sub_7100418AEC(sead::Vector3f* x0_0, const sead::Vector3f& a2, const IUsePlayerCollision* a3,
                    const sead::Vector3f& a4) {
    if (!rs::isCollidedWallFace(a3))
        return;

    const sead::Vector3f& CollidedWallNormal = rs::getCollidedWallNormal(a3);
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&a1, CollidedWallNormal, *x0_0);
    sead::Vector3f v15 = {0.0f, 0.0f, 0.0f};
    f32 v11 = a4.dot(a1);
    al::verticalizeVec(&v15, CollidedWallNormal, a2);
    f32 v12 = v11 + (a4.dot(a2) * 0.9f);
    if (al::isNearZeroOrGreater(v12, 0.001f)) {
        *x0_0 -= a4 * v12;
    }
}

void PlayerActionGroundMoveControl::updateNormalMove() {
    float v3;
    float v5;
    float v7;
    float v8;
    float v9;
    float v11;
    float v12;
    float v13;
    float v14;
    int v15;
    int v16;
    float v18;
    float v20;
    int v21;
    bool v24;
    bool v25;
    PlayerActionTurnControl* v29;
    bool v36;
    const sead::Vector3f* v37;
    bool v43;
    bool v44;
    bool _68;
    bool v48;
    float A0;
    float v50;
    float mMaxSpeed;
    bool v52;
    bool v53;
    float v54;
    float v57;
    bool v58;
    bool isJudge;
    float v60;
    bool v66;
    bool v67;
    float v69;
    float C0;
    float v71;
    float v83;
    float mMinSpeed;
    float v85;
    float v118;
    sead::Vector3f a2;
    sead::Vector3f _50;
    sead::Vector3f v124;
    sead::Vector3f a1;
    sead::Vector3f v126;
    sead::Vector3f a3;
    sead::Vector3f v128;

    if (mJudgeStartDash)
        rs::updateJudge(mJudgeStartDash);

    this->_64 = 0;
    this->_80 = 0;
    this->_BC = 0;
    v128 = {0.0f, 0.0f, 0.0f};
    updateNormalAndSnap(&v128);
    v3 = v128.length();
    if (v3 < 0.1f) {
        v128 = {0.0f, 0.0f, 0.0f};
    }

    a3 = {0.0f, 0.0f, 0.0f};
    al::tryNormalizeOrZero(&a3, v128);
    v5 = this->mConst->getDashBorderSpeed() - this->mConst->getDashBlendRange();
    v7 = this->mConst->getDashBorderSpeed();
    v8 = this->mConst->getDashBlendRange();
    v9 = sead::Mathf::clamp(1.0f - al::calcRate01(v3, v5, v7 + v8), 0.0f, 1.0f);

    v11 = this->mConst->getRoundLimitDegreeMin();
    v12 = this->mConst->getRoundLimitDegree();
    v13 = al::lerpValue(v11, v12, v9);
    v14 = this->mConst->getRoundFastDegree();
    v15 = this->mConst->getRoundAccelFrameFast();
    v16 = this->mConst->getRoundBrakeFrame();
    if (this->_9C) {
        v18 = this->mConst->getRoundLimitDegreeForce();
        v13 = this->mConst->getRoundLimitDegreeForceFast();
        v14 = this->mConst->getRoundFastDegreeForce();
        v15 = this->mConst->getRoundAccelFrameForceFast();
        v16 = this->mConst->getRoundBrakeFrameForce();
    } else {
        v18 = v13;
    }

    if (this->someFlags[2])
        v13 = 0.0;

    if (this->someFlags[2])
        v18 = 0.0;

    v20 = this->mConst->getRoundMinDegree();
    v21 = this->mConst->getRoundAccelFrame();
    mTurnControl->setup(v20, v14, v18, v13, v21, v15, v16);
    this->mTurnControl->_88 = this->someFlags[1] || this->_9C;
    v126 = {0.0f, 0.0f, 0.0f};
    if (_20)
        rs::calcHackerMoveVec(&v126, *_20, this->mGravityDir);
    else
        mInput->calcMoveInput(&v126, this->mGravityDir);

    this->_84 = v126;
    a1 = {0.0f, 0.0f, 0.0f};
    v124 = {0.0f, 0.0f, 0.0f};
    v24 = al::tryNormalizeOrZero(&a1, v126);
    _50 = {0.0f, 0.0f, 0.0f};
    v25 = this->someFlags[0];
    mTurnControl->update(v126, this->mGravityDir);

    sead::Vector3f xyz;
    if (v25 || !v24) {
        v29 = this->mTurnControl;
        xyz = v29->_5c;
        v124 = xyz;
        _50 = xyz;
    } else {
        xyz = a1;
        v124 = a1;
        _50 = this->mTurnControl->_50;
    }

    if (this->_9C) {
        if (v24) {
            v126 = a1;
            v24 = 1;
        } else {
            v24 = 1;
            v126 = xyz;
            a1 = xyz;
        }
    }

    v118 = v3;
    sead::Vector3f v33v;
    if (rs::isCollidedWall(this->mCollision)) {
        v33v = rs::getCollidedWallNormal(this->mCollision);
    } else {
        v33v = sead::Vector3f::zero;
    }

    v36 = al::isNearZero(_50, 0.001f);
    v43 = 0;
    v44 = !v24;
    if (!v36 && !v44)
        v43 = PlayerActionFunction::isOppositeDir(a1, _50) && !this->_9C;

    _68 = this->mTurnControl->_68;
    v48 = this->_D5 && !this->_20 && mInput->isHoldSquat();
    A0 = v118;
    v50 = _50.dot(a3);
    mMaxSpeed = 0.0;
    v52 = v33v.dot(a3) < -0.17365f && !v48;
    updateHillAffect(*v37, v124, v24);
    v53 = _68 | v44;
    if (v50 >= 0.0 && !(_68 | v44)) {
        v54 = v126.length();
        if (this->_78) {
            if (_7C <= 0.0)
                v57 = -_7C * mConst->getHillSubSpeed();
            else
                v57 = _7C * mConst->getHillAddSpeed();
        } else {
            v57 = 0.0;
        }

        if (this->mJudgeStartDash &&
            (v58 = al::isNearZeroOrLess((float)(this->mMaxSpeed * 0.99f) - v118, 0.001f),
             isJudge = rs::isJudge(this->mJudgeStartDash), v58) &&
            (isJudge & 1) != 0) {
            v60 = v57 + this->mDashMaxSpeed2D;
        } else {
            v60 = PlayerActionFunction::accel(v57 + this->mMaxSpeed, this->somethingAboutSpeed2,
                                              this->somethingAboutSpeed);
        }

        mMaxSpeed = (float)(v54 * v60) + (float)((float)(1.0f - v54) * this->mMinSpeed);
        if (v52 && mMaxSpeed > this->mMaxSpeed)
            mMaxSpeed = this->mMaxSpeed;

        A0 = PlayerActionFunction::accel(v118, mMaxSpeed,
                                         PlayerActionGroundMoveControl::calcAccelRate(v118));
    }

    if (A0 <= this->mBrakeOnSpeed) {
        if (_60 < mBrakeOnCounterCorner) {
            this->_60 = 0;
        }
    } else {
        this->_60 = sead::Mathf::clampMax(this->_60 + 1, this->mBrakeOnCounterCorner);
    }

    if (v52)
        goto LABEL_76;

    if (v50 < 0.0 && v43)
        goto LABEL_75;

    if (!(v44 || (v50 >= 0.0)))
        goto LABEL_76;

    if (v43 || v44) {
        if (!(_60 >= mBrakeOnCounterCorner || v48)) {
        LABEL_76:
            mNormalBrakeFrame = 1;
        }
        else {
        LABEL_75:;
        }
    } else {
        mNormalBrakeFrame = this->mStickOnBrakeFrame;
    }

    if (A0 > mMaxSpeed) {
        A0 = sead::Mathf::max(PlayerActionFunction::brake(A0, mNormalBrakeFrame, this->mMaxSpeed), mMaxSpeed);
    }

    if (this->_9C && A0 < this->_A0)
        A0 = this->_A0;

    if (v53 && !v36 && !v48 && this->_60 >= this->mBrakeOnCounterCorner) {
        updatePoseUpFront(this->mGravityDir, _50, A0);
        a2 = (A0 * _50) - (this->mGravity * this->mGravityDir);
        al::setVelocity(mActor, a2);
        this->_64 = 1;
        _90 = _50;
        return;
    }

    if (this->someFlags[0] && v43 && !v36 && (v50 < 0.0 || A0 <= this->mMinSpeed)) {
        updatePoseUpFront(this->mGravityDir, _50, A0);
        al::setVelocity(this->mActor, (A0 * a3) - (this->mGravity * this->mGravityDir));
        this->_BC = 1;
        _90 = _50;
        return;
    }

    v66 = al::isNearZero(A0, 0.001);
    v67 = !v24;
    if (!v66)
        v67 = 0;

    this->_80 = v67;
    if (this->_BD) {
        if (al::isNormalize(a3, 0.001f)) {
            v69 = sead::Mathf::abs(mTurnControl->calcTurnPowerRate(this->mGravityDir));

            C0 = this->_C0;
            v71 = sead::Mathf::clamp(v69 - C0, 0.0f, 1.0f);

            this->_C0 = al::lerpValue(C0, v69, 0.15f);
            a2 = {0.0f, 0.0f, 0.0f};
            al::verticalizeVec(&a2, v124, a3);
            if (al::tryNormalizeOrZero(&a2)) {
                v83 = al::lerpValue(0.3f, 0.9f, v71);
                mMinSpeed = A0 * v124.dot(a3);
                if (mMinSpeed < this->mMinSpeed)
                    mMinSpeed = this->mMinSpeed;

                v85 = a3.dot(a2);
                sead::Vector3f v121 = (v124 * mMinSpeed) + (v83 * (A0 * (a2 * v85)));
                f32 v121_len = v121.length();
                if (v121_len > 0.0f) {
                    f32 factor = A0 / v121_len;
                    v121 *= factor;
                }

                sub_7100418AEC(&v121, this->mGravityDir * -this->mGravity, mCollision, -al::getGravity(this->mActor));
                al::setVelocity(this->mActor, v121 - (this->mGravity * this->mGravityDir));
                goto LABEL_110;
            }
        } else {
            this->_C0 = 0.0;
        }
    }

    a2 = A0 * v124;
    sub_7100418AEC(&a2, this->mGravityDir * -this->mGravity, this->mCollision,
                   -al::getGravity(this->mActor));
    al::setVelocity(this->mActor, a2 - (this->mGravity * this->mGravityDir));

LABEL_110:
    a2 = this->mGravityDir;
    if (this->_BB)
        this->mTurnControl->calcTilt(&a2, this->mGravityDir, this->mConst->getTiltPoseDegreeMax());

    updatePoseUpFront(a2, v124, A0);
    this->_90 = v124;
}
void PlayerActionGroundMoveControl::updateNormalAndSnap(sead::Vector3f* a2) {
    sead::Vector3f* p_mGravityDir;
    float y;
    float v8;
    float v9;
    sead::Vector3f* VelocityPtr;
    float v17;
    sead::Vector3f v19;
    sead::Vector3f a2a;
    sead::Vector3f a3;

    p_mGravityDir = &this->mGravityDir;
    a3 = this->mGravityDir;
    if (!this->_C4 || rs::isCollidedGround(this->mCollision)) {
        rs::calcGroundNormalOrGravityDir(p_mGravityDir, this->mActor, this->mCollision);

    LABEL_4:
        y = this->mGravityDir.y;
        this->someVec.x = this->mGravityDir.x;
        this->someVec.y = y;
        this->someVec.z = this->mGravityDir.z;
        goto LABEL_5;
    }

    this->mGravityDir = -al::getGravity(mActor);
    a2a = {0.0f, 0.0f, 0.0f};
    v19 = {0.0f, 0.0f, 0.0f};

    if (alCollisionUtil::getHitPosAndNormalOnArrow(mActor, &a2a, &v19, al::getTrans(this->mActor),
                                                   al::getGravity(this->mActor) * 100.0f, 0LL,
                                                   0LL)) {
        if ((al::isFloorPolygon(v19, al::getGravity(this->mActor)) & 1) != 0) {
            if (someVec.dot(v19) <= 0.087156) {
                VelocityPtr = al::getVelocityPtr(this->mActor);
                v17 = al::calcSpeedH(this->mActor);
                al::limitLength(VelocityPtr, *VelocityPtr, v17);
                goto LABEL_5;
            }

            al::turnVecToVecRate(p_mGravityDir, a3, v19, 0.5);
            goto LABEL_4;
        }
    }

LABEL_5:
    al::alongVectorNormalH(a2, al::getVelocity(this->mActor), a3, *p_mGravityDir);
}
f32 PlayerActionGroundMoveControl::calcTurnTiltRate() const {
    if (rs::isOnGroundSkateCode(this->mActor, this->mCollision))
        return 0.0;
    return mTurnControl->calcTurnPowerRate(this->mGravityDir);
}
void PlayerActionGroundMoveControl::calcMoveInput(sead::Vector3f*, sead::Vector3f const&) {
    CRASH
}
bool PlayerActionGroundMoveControl::isActiveSquatBrake() const {
    CRASH
}
void PlayerActionGroundMoveControl::updateHillAffect(sead::Vector3f const& a2,
                                                     sead::Vector3f const& a3, bool a4) {
    const sead::Vector3f* Gravity;
    float v8;
    float v9;
    float v10;
    const sead::Vector3f* v11;
    const PlayerConst** p_mConst;
    float v13;
    float v14;
    float v15;
    float v16;
    float v17;
    bool v18;
    float* p__7C;
    float v20;
    float _7C;
    float v22;
    int v23;
    long double v24;
    float v25;
    float v26;
    bool v27;
    float v28;
    float v29;
    float v30;
    float v31;
    float v32;
    float v33;
    sead::Vector3f v34;

    auto* a1 = this;

    if (a1->_78) {
        Gravity = &al::getGravity(a1->mActor);
        v8 = (float)((float)-(float)(Gravity->y * a1->mGravityDir.y) -
                     (float)(Gravity->x * a1->mGravityDir.x)) -
             (float)(Gravity->z * a1->mGravityDir.z);
        v9 = -1.0;
        if (v8 >= -1.0) {
            v9 = (float)((float)-(float)(Gravity->y * a1->mGravityDir.y) -
                         (float)(Gravity->x * a1->mGravityDir.x)) -
                 (float)(Gravity->z * a1->mGravityDir.z);
            if (v8 > 1.0)
                v9 = 1.0;
        }

        v34.x = 0.0;
        v34.y = 0.0;
        v34.z = 0.0;
        v10 = sead::Mathf::acos(v9);
        v11 = &al::getGravity(a1->mActor);
        al::verticalizeVec(&v34, a1->mGravityDir, *v11);
        if (al::tryNormalizeOrZero(&v34)) {
            p_mConst = &a1->mConst;
            v13 = v10 * 57.296;
            v14 = (float)(v10 * 57.296) / a1->mConst->getStandAngleMin();
            v15 = 0.0;
            if (v14 >= 0.0) {
                v15 = v14;
                if (v14 > 1.0)
                    v15 = 1.0;
            }

            v16 = 0.0;
            if (a4 && (v16 = v34.dot(a3), v16 > 0.0))
                v17 = (float)(v16 * al::easeOut(v15)) + 0.0;
            else
                v17 = 0.0 - al::easeIn(v15);

            v20 = fmaxf(v13 * v16, 0.0);
            _7C = a1->_7C;
            p__7C = &a1->_7C;
            v22 = _7C;
            v18 = v17 > 0.0;
            if (v17 > 0.0 && v22 < 0.0) {
                v18 = 1;
                *p__7C = 0.0;
            }
        } else {
            v17 = 0.0;
            v18 = 0;
            p_mConst = &a1->mConst;
            p__7C = &a1->_7C;
            v20 = 0.0;
        }

        v23 = (*p_mConst)->getHillAccelAddFrame();
        if (*p__7C > v17) {
            v24 = 1.0;
            if ((float)(v17 + 1.0) <= 1.0)
                *(float*)&v24 = v17 + 1.0;

            if ((float)(v17 + 1.0) >= 0.0)
                v25 = *(float*)&v24;
            else
                v25 = 0.0;

            v26 = v25 * ((*p_mConst)->getHillAccelSubFrame() - 1);
            v27 = v26 >= 0.0;
            if ((float)(int)v26 == v26)
                v27 = 0;

            v23 = (int)v26 + v27 + 1;
            if (v18) {
                v28 = (*p_mConst)->getHillAccelSubAngleMin();
                v29 = (*p_mConst)->getHillAccelSubAngleMax();
                v30 = al::calcRate01(v20, v28, v29);
                v31 = *p__7C;
                v32 = al::easeOut(v30);
                v33 = al::easeOut(v32);
                v17 = al::lerpValue(v17, v31, v33);
            }
        }

        *p__7C = al::converge(*p__7C, v17, 1.0 / (float)v23);
    }
}
f32 PlayerActionGroundMoveControl::calcMaxSpeed(float a2) const {
    float _7C;
    const PlayerConst* v5;
    float v6;
    bool v7;
    char isJudge;

    if (this->_78) {
        _7C = this->_7C;
        v5 = this->mConst;
        if (_7C <= 0.0)
            v6 = -(float)(_7C * v5->getHillSubSpeed());
        else
            v6 = _7C * v5->getHillAddSpeed();
    } else {
        v6 = 0.0;
    }

    if (this->mJudgeStartDash &&
        (v7 = al::isNearZeroOrLess((float)(this->mMaxSpeed * 0.99) - a2, 0.001),
         isJudge = rs::isJudge(this->mJudgeStartDash), v7) &&
        (isJudge & 1) != 0) {
        return v6 + this->mDashMaxSpeed2D;
    } else {
        return PlayerActionFunction::accel(v6 + this->mMaxSpeed, this->somethingAboutSpeed2,
                                           this->somethingAboutSpeed);
    }
}
f32 PlayerActionGroundMoveControl::calcAccelRate(float a2) const {
    IJudge* mJudgeStartDash;  // x0
    float v5;                 // s0
    bool v6;                  // w20
    int isJudge;              // w0
    float mMaxSpeed;          // s0
    float mDashAccelFrame2D;  // s1
    int mAccelFrame;          // w8
    float v11;                // s10
    float v12;                // s9
    float v13;                // s10

    mJudgeStartDash = this->mJudgeStartDash;
    if (!mJudgeStartDash ||
        ((v5 = this->mMaxSpeed - a2, v5 <= 0.0) ?
             (IJudge*)(v6 = 0) :
             (v6 = !al::isNearZero(v5, 0.001), mJudgeStartDash = this->mJudgeStartDash),
         (isJudge = rs::isJudge(mJudgeStartDash), v6) || ((isJudge ^ 1) & 1) != 0)) {
        mMaxSpeed = this->mMaxSpeed;
        mAccelFrame = this->mAccelFrame;
        if (this->_D4 && mMaxSpeed > a2) {
            v11 = (float)(mAccelFrame * mAccelFrame);
            v12 = (float)(mMaxSpeed / (float)mAccelFrame) * this->mConst->getRunAccelAverageScale();
            v13 = (float)((float)(this->mMaxSpeed - (float)(v12 * (float)this->mAccelFrame)) +
                          (float)(this->mMaxSpeed - (float)(v12 * (float)this->mAccelFrame))) /
                  v11;
            return v12 +
                   (float)(v13 * (float)((float)((float)(sqrtf((float)(v12 * v12) +
                                                               (float)((float)(v13 + v13) * a2)) -
                                                         v12) /
                                                 v13) +
                                         1.0));
        }

        mDashAccelFrame2D = (float)mAccelFrame;
    } else {
        mMaxSpeed = this->mDashMaxSpeed2D - this->mMaxSpeed;
        mDashAccelFrame2D = (float)this->mDashAccelFrame2D;
    }

    return mMaxSpeed / mDashAccelFrame2D;
}
void PlayerActionGroundMoveControl::updatePoseUpFront(sead::Vector3f const& a2,
                                                      sead::Vector3f const& a3, float a4) {
    float v7;               // s8
    al::LiveActor* mActor;  // x21
    float v9;               // s9
    float v10;              // s0
    sead::Vector3f v11;     // [xsp+0h] [xbp-40h] BYREF

    v11 = a3;
    if (al::isParallelDirection(a2, v11, 0.01)) {
        al::calcFrontDir(&v11, this->mActor);
        if (al::isParallelDirection(a2, v11, 0.01))
            al::calcUpDir(&v11, this->mActor);
    }

    if (this->_A4)
        v7 = al::calcRate01(a4, this->_A8, this->_AC);
    else
        v7 = 1.0;

    mActor = this->mActor;
    v9 = this->mConst->getSlerpQuatRate();
    v10 = this->mConst->getHillPoseDegreeMax();
    rs::slerpUpFront(mActor, a2, v11, v9, v7 * v10);
}
