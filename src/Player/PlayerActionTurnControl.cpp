#include "Player/PlayerActionTurnControl.h"

#include <basis/seadNew.h>

#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathLerpUtil.h"
#include "Library/Math/MathQuatUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"

#include "Player/PlayerActionFunction.h"

PlayerActionTurnControl::PlayerActionTurnControl(al::LiveActor* liveActor) : mLiveActor(liveActor) {
    float* arr = new (8, std::nothrow) float[5];
    if (arr) {
        _48 = 0;
        this->mArr = arr;
        mArrSize = 5;
        _44 = 0;
    }
}

void PlayerActionTurnControl::setup(float turnAngleStart, float turnAngleFast, float turnAngleLimit,
                                    float turnAngleFastLimit, int turnAccelFrame,
                                    int turnAccelFrameFast, int turnBrakeFrame) {
    mTurnAngleStart = turnAngleStart;
    mTurnAngleFast = turnAngleFast;
    mTurnAngleLimit = turnAngleLimit;
    mTurnAngleFastLimit = turnAngleFastLimit;
    mTurnAccelFrame = turnAccelFrame;
    mTurnAccelFrameFast = turnAccelFrameFast;
    mTurnBrakeFrame = turnBrakeFrame;
}

void PlayerActionTurnControl::reset() {
    _44 = 0;
    _48 = 0;
    _30 = 0.0;
    _8 = 0.0;
    _c = {0, 0, 0};
    _18 = {0, 0, 0};
    _24 = {0, 0, 0};
}

void PlayerActionTurnControl::update(const sead::Vector3f& a2, const sead::Vector3f& gravity) {
    sead::Vector3f front = {0, 0, 0};
    al::calcFrontDir(&front, mLiveActor);

    sead::Vector3f up = {0, 0, 0};
    al::calcUpDir(&up, mLiveActor);

    sead::Vector3f& v1 = front;  // get new name, but causes more mismatches if using new variable
    al::alongVectorNormalH(&v1, front, up, gravity);
    al::tryNormalizeOrZero(&v1);
    _50 = v1;

    sead::Vector3f v99 = {0, 0, 0};
    if (!al::isNearZero(_c, 0.001f) || !al::isNearZero(_24, 0.001f)) {
        al::alongVectorNormalH(&v99, _c, _24, gravity);
        al::tryNormalizeOrZero(&v99);
    }

    sead::Vector3f normalizedGravity = {0, 0, 0};
    bool hasGravity = al::tryNormalizeOrZero(&normalizedGravity, gravity);
    
    sead::Vector3f actorGravity = al::getGravity(mLiveActor);
    bool v89 = up.dot(-actorGravity) < 0.087156f && _89;

    bool v14 = al::isNearZero(v1, 0.001f);
    bool v15 = v89 || v14;
    bool v16 = al::isNearZero(v99, 0.001f);
    bool v17 = hasGravity && !v16;
    bool v18 = v16 || !hasGravity;

    float v19;
    if (v18 || normalizedGravity.dot(v99) < 0.34202f) {
        _44 = 0;
        _48 = 0;
        _30 = 0.0;
        v19 = 0.0;
    } else {
        float v20;
        if (hasGravity && !v16) {
            v20 = sead::Mathf::abs(al::calcAngleDegree(v99, normalizedGravity));
        } else {
            v20 = 0.0;
        }

        int v48 = _48;
        int loc_arrsize = mArrSize;
        int v44 = _44;
        int v24;
        if (v48 < loc_arrsize)
            goto LABEL_21;

        v24 = v48 - 1;
        if (v48 >= 1) {
            int v25 = _44;
            int v26;
            if (v25 + 1 < loc_arrsize)
                v26 = v25 + 1;
            else
                v26 = 0;

            v48 = v24;
            _44 = v26;
            _48 = v24;
            loc_arrsize = mArrSize;
            v44 = _44;
        }

        if (v48 < loc_arrsize) {
        LABEL_21:
            int v27 = v48 + 1;
            int v28 = v44 + v48;
            if (v28 < loc_arrsize)
                loc_arrsize = 0;

            int v29 = v28 - loc_arrsize;
            _48 = v27;
            mArr[v29] = v20;
        }

        int v31 = _48;
        _30 = 0.0;
        int v32 = std::min(v31, 5);

        // __OFSUB__ ?
        int v33 = v31 - 1;
        int v34 = v31 - 1;
        float v38 = 0.0;
        if (v34 < 0 == v33) {
            int v35 = mArrSize;
            int v36 = _44;
            int i = 0;
            do {
                int v40;
                if (v34 + v36 >= v35)
                    v40 = v35;
                else
                    v40 = 0;

                int v41 = v34 + v36 - v40;
                --v34;
                v38 += mArr[v41];
                _30 = v38;
            } while (++i < v32);
        }

        v19 = v38 / 5.0;
        _30 = v38 / 5.0;
    }

    bool v42, v43, v44;
    if (v89 || v14 || !hasGravity) {
        v42 = false;
        v43 = false;
        v44 = hasGravity;
    } else {
        bool isOppositeDir = PlayerActionFunction::isOppositeDir(normalizedGravity, v1);
        v42 = false;
        if (v18 || (v17 && v19 <= 1.0f)) {
            v44 = hasGravity;
            v43 = false;
            if (isOppositeDir) {
                // there's more complicated stuff here
                v43 = _88;
                v42 = !_88;
                v44 = hasGravity & v43;
            }
        } else {
            v44 = hasGravity;
            v43 = false;
        }
    }

    _68 = v42;
    _69 = v43;
    _6a = v89;

    sead::Vector3f v98 = {0, 0, 0};
    // also seems more complicated
    if ((v44 & (v15 ^ 1) & 1) == 0) {
        if (hasGravity) {
            if (v42) {
                v98 = v1;
                _8 = 0.0f;
            } else {
                v98 = normalizedGravity;
                if (_89)
                    _8 = mTurnAngleFastLimit;
            }
        } else if (!_89) {
            if (v15)
                al::calcFrontDir(&v98, mLiveActor);
            else
                v98 = v1;

            _c = {0, 0, 0};
            _18 = {0, 0, 0};
            _24 = {0, 0, 0};
            _8 = PlayerActionFunction::brake(_8, mTurnBrakeFrame, mTurnAngleLimit);
        } else {
            al::calcFrontDir(&v98, mLiveActor);
            if (up.dot(-actorGravity) < 0.0f) {
                v98 *= (-1);
            }

            _c = {0, 0, 0};
            _18 = {0, 0, 0};
            _24 = {0, 0, 0};
            _8 = mTurnAngleFastLimit;
        }

        _5c = v98;
        return;
    }

    float v48;
    if (v17) {
        v48 = v99.dot(normalizedGravity);
    } else {
        v48 = v1.dot(normalizedGravity);
    }

    float v49 = sead::Mathf::clamp(v48, -1.0f, 1.0f);
    float v50 = sead::Mathf::cos(v49);
    float v53 = sead::Mathf::clamp(v1.dot(normalizedGravity), -1.0f, 1.0f);
    float v54 = sead::Mathf::cos(v53);
    sead::Vector3f v97 = v99.cross(normalizedGravity);
    al::tryNormalizeOrZero(&v97);

    bool v57, v58;
    sead::Vector3f v95;
    if (!v17 || al::isNearZero(_18, 0.001f)) {
        v57 = false;
        v58 = false;
    } else {
        float v55 = sead::Mathf::rad2deg(v50);
        if (v97.dot(_18) <= 0.0f) {
            v57 = false;
            if (v55 >= sead::Mathf::max(mTurnAngleStart, _8)) {
                _8 = 0.0f;
                v58 = false;
            } else {
                _8 = al::converge(_8, 0.0f, v55);
                v58 = v55 < mTurnAngleStart;
            }
        } else {
            v95 = v1.cross(normalizedGravity);
            if (!al::tryNormalizeOrZero(&v95) || v95.dot(v97) >= 0.0f) {
                v57 = false;
                v58 = false;
            } else {
                v57 = v19 > 10.0f || v55 > 3.0f;
                v58 = v19 <= 10.0f && v55 <= 3.0f;
            }
        }
    }

    float v59 = sead::Mathf::rad2deg(v54);
    float v60 = al::calcRate01(gravity.length(), 0.0f, 0.8f);
    float v61 = sead::Mathf::clamp(v60, 0.25f, 1.0f);

    if (v59 < mTurnAngleStart) {
        _8 = PlayerActionFunction::brake(_8, mTurnBrakeFrame, mTurnAngleLimit);
    } else if (v59 >= mTurnAngleFast) {
        _8 = PlayerActionFunction::accel(_8, mTurnAngleFastLimit,
                                         mTurnAngleFastLimit / (float)mTurnAccelFrameFast);
    } else {
        f32 x2 = v61 * mTurnAngleLimit;
        if (_8 >= x2) {
            _8 = PlayerActionFunction::brake(_8, mTurnBrakeFrame, mTurnAngleFastLimit);
        } else {
            _8 = PlayerActionFunction::accel(_8, x2, x2 / (float)mTurnAccelFrame);
        }
    }

    sead::Vector3f v73v;
    sead::Vector3f v96 = {0, 0, 0};
    if (v57 || al::isReverseDirection(v1, normalizedGravity, 0.01f)) {
        sead::Vector3f v67v = gravity;
        if (v97.dot(gravity) <= 0.0) {
            v67v = -v67v;
        }

        float v72 = sead::Mathf::deg2rad(_8) * 0.5f;
        float cos = sead::Mathf::cos(v72);
        float sin = sead::Mathf::sin(v72);
        sead::Vector3f v78v = v67v * sin;

        sead::Vector3f helper = v78v.cross(v1) + (cos * v1);
        float v84 = -v78v.dot(v1);
        v98 = v78v.cross(helper) + (cos * helper) - (v78v * v84);
        al::tryNormalizeOrZero(&v98);

        v73v = v67v;
        v96 = v67v;
    } else {
        sead::Quatf quat = {0, 0, 0, 1};
        al::makeQuatRotationLimit(&quat, v1, normalizedGravity, sead::Mathf::deg2rad(_8));
        sead::Vector3f v78v = {quat.x, quat.y, quat.z};
        float cos = quat.w;

        sead::Vector3f helper = v78v.cross(v1) + (cos * v1);
        float v84 = -v78v.dot(v1);
        v98 = v78v.cross(helper) + (cos * helper) - (v78v * v84);
        al::normalize(&v98);

        v96 = v1.cross(normalizedGravity);
        al::tryNormalizeOrZero(&v96);

        if (v58 && v96.dot(_18) < 0.0f) {
            v96 = -v96;
        }
        v73v = v96;
    }

    _18 = v73v;
    _c = normalizedGravity;
    _24 = gravity;
    _5c = v98;
}

// NON_MATCHING: floating-point math
void PlayerActionTurnControl::calcTilt(sead::Vector3f* a1, const sead::Vector3f& a2, float a3) {
    sead::Vector3f local5c = _5c;
    float turnPowerRate = calcTurnPowerRate(a2);

    float v13 = sead::Mathf::deg2rad(turnPowerRate * a3) * 0.5f;
    float v14 = sead::Mathf::cos(v13);
    float v15 = sead::Mathf::sin(v13);

    sead::Vector3f sin5c = v15 * local5c;
    sead::Vector3f another = sin5c.cross(a2) + (v14*a2);
    float val = -sin5c.dot(a2);
    *a1 = sin5c.cross(another) + (v14*another) - (sin5c*val);
}

float PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a1) const {
    float result = al::easeIn(al::calcRate01(_8, mTurnAngleStart, mTurnAngleLimit));
    if (a1.dot(_18) > 0) {
        return -result;
    }
    return result;
}
