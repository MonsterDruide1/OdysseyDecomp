#include "Player/PlayerActionTurnControl.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "math/seadQuat.h"

PlayerActionTurnControl::PlayerActionTurnControl(al::LiveActor* actor) : mPlayer(actor) {
    _38.allocBuffer(5, nullptr);
}


void PlayerActionTurnControl::setup(f32 turnAngleStart, f32 turnAngleFast, f32 turnAngleLimit,
                                    f32 turnAngleFastLimit, s32 turnAccelFrame,
                                    s32 turnAccelFrameFast, s32 turnBrakeFrame) {
    mTurnAngleStart = turnAngleStart;
    mTurnAngleFast = turnAngleFast;
    mTurnAngleLimit = turnAngleLimit;
    mTurnAngleFastLimit = turnAngleFastLimit;
    mTurnAccelFrame = turnAccelFrame;
    mTurnAccelFrameFast = turnAccelFrameFast;
    mTurnBrakeFrame = turnBrakeFrame;
}

void PlayerActionTurnControl::reset() {
    _38.clear();
    _30 = 0.0;
    _8 = 0.0;
    _c = {0.0f, 0.0f, 0.0f};
    _18 = {0.0f, 0.0f, 0.0f};
    _24 = {0.0f, 0.0f, 0.0f};
}

void PlayerActionTurnControl::update(const sead::Vector3f& a2, const sead::Vector3f& a3) {
    bool v12;
    bool v13;
    bool v14;
    bool v15;
    bool v16;
    bool v17;
    bool v18;
    float v19;
    float v20;
    float v38;
    bool v42;
    bool v43;
    bool v44;
    bool isOppositeDir;
    float v48;
    float v49;
    float v50;
    sead::Vector3f* v51;
    float v53;
    float v54;
    float v55;
    const sead::Vector3f* v56;
    bool v57;
    bool v58;
    float v59;
    float v60;
    float v61;
    float lTurnAngleLimit;
    float lTurnAngleFastLimit;
    int lTurnAccelFrame;
    f32 v73, v74, v75;
    float v81;
    float v82;
    float v83;
    float v84;
    float v85;
    float v86;
    float v87;
    float v88;
    float v89;
    float v90;
    float v91;
    const sead::Vector3f* v94;
    sead::Vector3f v67v;
    sead::Quatf v95q;
    sead::Vector3f v95v;
    sead::Vector3f v96;
    sead::Vector3f v97;
    sead::Vector3f v98;
    sead::Vector3f a3a;
    sead::Vector3f a1;
    sead::Vector3f* v103;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, this->mPlayer);
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    al::calcUpDir(&up, this->mPlayer);
    al::alongVectorNormalH(&front, front, up, a3);
    al::tryNormalizeOrZero(&front);
    this->_50 = front;

    a1 = {0.0f, 0.0f, 0.0f};
    if (!al::isNearZero(this->_c, 0.001) || !al::isNearZero(this->_24, 0.001)) {
        al::alongVectorNormalH(&a1, this->_c, this->_24, a3);
        al::tryNormalizeOrZero(&a1);
    }

    v94 = &a3;
    sead::Vector3f neggravity = -al::getGravity(this->mPlayer);
    a3a = {0.0f, 0.0f, 0.0f};
    v12 = al::tryNormalizeOrZero(&a3a, a2);
    v13 = false;
    if (up.dot(neggravity) < 0.087156f)
        v13 = this->_89;

    v14 = al::isNearZero(front, 0.001f);
    v15 = v13 || v14;
    v16 = al::isNearZero(a1, 0.001f);
    v17 = v12 && !v16;
    v18 = v16 || !v12;
    v103 = (sead::Vector3f*)&a2;
    if (v18 || a3a.dot(a1) < 0.34202f) {
        this->_38.clear();
        v19 = 0.0;
        this->_30 = 0.0;
    } else {
        if (v17)
            v20 = sead::Mathf::abs(al::calcAngleDegree(a1, a3a));
        else
            v20 = 0.0;

        _38.forcePushBack(v20);

        this->_30 = 0.0f;
        v38 = 0.0f;
        for (int i = 0; i < _38.size(); i++) {
            v38 += _38[i];
            this->_30 = v38;
        }

        v19 = v38 / 5.0f;
        this->_30 = v38 / 5.0f;
    }

    if (v15 || !v12) {
        v42 = 0;
        v43 = 0;
        v44 = v12;
    } else {
        isOppositeDir = PlayerActionFunction::isOppositeDir(a3a, front);
        v42 = false;
        if (v18 || (v17 && v19 <= 1.0)) {
            v44 = v12;
            v43 = false;
            if (isOppositeDir) {
                v43 = this->_88;
                v42 = !this->_88;
                v44 = v12 && v43;
            }
        } else {
            v44 = v12;
            v43 = false;
        }
    }

    this->_69 = v43;
    this->_68 = v42;
    this->_6a = v13;
    v98 = {0.0f, 0.0f, 0.0f};
    if ((v44 && !v15) == 0) {
        if (v12) {
            if (v42) {
                v98 = front;
                this->_8 = 0.0;
                this->_5c = v98;
                return;
            }

            v98 = a3a;
            if (!v13) {
                this->_5c = v98;
                return;
            }
        } else {
            if (!v13) {
                if (v15)
                    al::calcFrontDir(&v98, this->mPlayer);
                else
                    v98 = front;

                this->_18 = {0.0f, 0.0f, 0.0f};
                this->_24 = {0.0f, 0.0f, 0.0f};
                this->_c = {0.0f, 0.0f, 0.0f};
                this->_8 = PlayerActionFunction::brake(this->_8, this->mTurnBrakeFrame,
                                                       this->mTurnAngleLimit);
                this->_5c = v98;
                return;
            }

            al::calcFrontDir(&v98, this->mPlayer);
            if (up.dot(neggravity) < 0.0)
                v98 = -v98;

            this->_c = {0.0f, 0.0f, 0.0f};
            this->_18 = {0.0f, 0.0f, 0.0f};
            this->_24 = {0.0f, 0.0f, 0.0f};
        }

        this->_8 = this->mTurnAngleFastLimit;
        this->_5c = v98;
        return;
    }

    if (v17)
        v48 = a1.dot(a3a);
    else
        v48 = front.dot(a3a);

    v49 = sead::Mathf::clamp(v48, -1.0, 1.0);
    v50 = sead::Mathf::acos(v49);
    v51 = v103;
    v53 = sead::Mathf::clamp(front.dot(a3a), -1.0, 1.0);
    v54 = sead::Mathf::acos(v53);
    v97.setCross(a1, a3a);
    al::tryNormalizeOrZero(&v97);
    if (!v17 || al::isNearZero(this->_18, 0.001f))
        goto LABEL_61;

    v55 = sead::Mathf::rad2deg(v50);
    if (v97.dot(this->_18) <= 0.0) {
        if (v55 < sead::Mathf::max(mTurnAngleStart, _8)) {
            this->_8 = al::converge(this->_8, 0.0, v55);
            v57 = false;
            v58 = v55 < this->mTurnAngleStart;
        } else {
            v57 = false;
            v58 = false;
            this->_8 = 0.0;
        }

        v56 = v94;
    } else {
        v95v.setCross(front, a3a);
        if (!al::tryNormalizeOrZero(&v95v)) {
        LABEL_61:
            v57 = false;
            v58 = false;
            v56 = v94;
        } else {
            v56 = &a3;
            if (v95v.dot(v97) >= 0.0) {
                v57 = false;
                v58 = false;
            } else {
                v57 = v19 > 10.0f || v55 > 3.0f;
                v58 = v19 <= 10.0f && v55 <= 3.0f;
            }
        }
    }

    v59 = sead::Mathf::rad2deg(v54);
    v60 = al::calcRate01(v51->length(), 0.0f, 0.8f);
    v61 = sead::Mathf::clamp(v60, 0.25f, 1.0f);

    lTurnAngleLimit = this->mTurnAngleLimit;
    if (v59 < this->mTurnAngleStart) {
        this->_8 = PlayerActionFunction::brake(this->_8, this->mTurnBrakeFrame, lTurnAngleLimit);
    } else {
        if (v59 >= this->mTurnAngleFast) {
            lTurnAngleFastLimit = this->mTurnAngleFastLimit;
            lTurnAccelFrame = this->mTurnAccelFrameFast;
            this->_8 = PlayerActionFunction::accel(this->_8, lTurnAngleFastLimit,
                                            lTurnAngleFastLimit / lTurnAccelFrame);
        } else {
            lTurnAngleFastLimit = v61 * lTurnAngleLimit;
            if (this->_8 >= lTurnAngleFastLimit) {
                lTurnAngleLimit = this->mTurnAngleFastLimit;
                this->_8 = PlayerActionFunction::brake(this->_8, this->mTurnBrakeFrame, lTurnAngleLimit);
            } else {
                lTurnAccelFrame = this->mTurnAccelFrame;
                this->_8 = PlayerActionFunction::accel(this->_8, lTurnAngleFastLimit,
                                                lTurnAngleFastLimit / lTurnAccelFrame);
            }
        }
    }

    v96 = {0.0f, 0.0f, 0.0f};
    if (v57 || al::isReverseDirection(front, a3a, 0.01f)) {
        v67v = *v56;
        if (v97.dot(*v56) <= 0.0) {
            v67v = -v67v;
        }

        sead::Quatf quat;
        v73 = v67v.x;
        v74 = v67v.y;
        v75 = v67v.z;
        sead::QuatCalcCommon<f32>::setAxisAngle(quat, v67v, _8);
        v81 = ((quat.y * front.z) - (quat.z * front.y)) + (quat.w * front.x);
        v82 = (quat.w * front.y) + ((quat.z * front.x) - (front.z * quat.x));
        v83 = (quat.w * front.z) + ((quat.x * front.y) - (quat.y * front.x));
        v84 = (-(quat.x * front.x) - (quat.y * front.y)) - (front.z * quat.z);
        v98.x = ((quat.y * v83) + ((quat.w * v81) - (quat.z * v82))) - (quat.x * v84);
        v98.y = (((quat.z * v81) + (quat.w * v82)) - (quat.x * v83)) - (quat.y * v84);
        v98.z = ((quat.w * v83) + ((quat.x * v82) - (quat.y * v81))) - (quat.x * v84);
        al::tryNormalizeOrZero(&v98);
        v96 = v67v;
    } else {
        v95q = sead::Quatf::unit;
        al::makeQuatRotationLimit(&v95q, front, a3a, sead::Mathf::deg2rad(this->_8));
        v85 = ((v95q.y * front.z) - (v95q.z * front.y)) + (v95q.w * front.x);
        v86 = (front.y * v95q.w) + ((v95q.z * front.x) - (front.z * v95q.x));
        v87 = (front.z * v95q.w) + ((front.y * v95q.x) - (v95q.y * front.x));
        v88 = (-(front.x * v95q.x) - (v95q.y * front.y)) - (front.z * v95q.z);
        v98.x = ((v95q.y * v87) + ((v95q.w * v85) - (v95q.z * v86))) - (v95q.x * v88);
        v98.y = (((v85 * v95q.z) + (v86 * v95q.w)) - (v87 * v95q.x)) - (v88 * v95q.y);
        v98.z = (((v86 * v95q.x) - (v85 * v95q.y)) + (v87 * v95q.w)) - (v88 * v95q.z);
        al::normalize(&v98);
        v96.setCross(front, a3a);
        al::tryNormalizeOrZero(&v96);
        if (v58) {
            v89 = v96.x;
            v90 = v96.y;
            v91 = v96.z;
            if (v96.dot(this->_18) < 0.0) {
                v89 = -v96.x;
                v90 = -v96.y;
                v91 = -v96.z;
                v96 = -v96;
            }

            v73 = v89;
            v74 = v90;
            v75 = v91;
        } else {
            v73 = v96.x;
            v74 = v96.y;
            v75 = v96.z;
        }
    }

    this->_18.x = v73;
    this->_18.y = v74;
    this->_18.z = v75;
    this->_c = a3a;
    this->_24 = *v56;
    this->_5c = v98;
}

void PlayerActionTurnControl::calcTilt(sead::Vector3f*, const sead::Vector3f&, f32){
    CRASH} f32 PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a2) const {
    f32 result = al::easeIn(al::calcRate01(_8, mTurnAngleStart, mTurnAngleLimit));
    if (a2.dot(_18) > 0.0)
        return -result;

    return result;
}
