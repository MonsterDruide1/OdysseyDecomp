#include "Player/PlayerActionTurnControl.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "math/seadQuat.h"

PlayerActionTurnControl::PlayerActionTurnControl(al::LiveActor* actor) : mActor(actor) {
    _48 = 0;
    arr = new f32[5];
    arr_size = 5;
    _44 = 0;
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
    _44 = 0;
    _48 = 0;
    _30 = 0.0;
    _8 = 0.0;
    _c = {0.0f, 0.0f, 0.0f};
    _18 = {0.0f, 0.0f, 0.0f};
    _24 = {0.0f, 0.0f, 0.0f};
}
void PlayerActionTurnControl::update(const sead::Vector3f& a2, const sead::Vector3f& a3) {
    sead::Vector3f* p_c;            // x22
    const sead::Vector3f* Gravity;  // x0
    float x;                        // s0
    float y;                        // s10
    float z;                        // s11
    float v11;                      // s9
    bool v12;                       // w23
    bool v13;                       // w26
    bool v14;                       // w24
    bool v15;                       // w27
    bool v16;                       // w0
    bool v17;                       // w25
    bool v18;                       // w28
    float v19;                      // s12
    float v20;                      // s0
    int _48;                        // w9
    int arr_size;                   // w8
    int _44;                        // x8^4
    int v24;                        // w10
    int v25;                        // w9
    int v26;                        // w8
    int v27;                        // w11
    int v28;                        // w9
    int v29;                        // w8
    float* arr;                     // x9
    int v31;                        // w9
    int v32;                        // w8
    bool v33;                       // vf
    int v34;                        // w9
    int v35;                        // w11
    int v36;                        // w12
    float* v37;                     // x13
    float v38;                      // s0
    int v39;                        // w10
    int v40;                        // w14
    int v41;                        // w14
    bool v42;                       // w8
    bool v43;                       // w10
    bool v44;                       // w9
    bool isOppositeDir;             // w0
    float v46;                      // s0
    float v47;                      // s1
    float v48;                      // s1
    float v49;                      // s0
    float v50;                      // s10
    sead::Vector3f* v51;            // x21
    float v52;                      // s1
    float v53;                      // s0
    float v54;                      // s9
    float v55;                      // s8
    const sead::Vector3f* v56;      // x25
    bool v57;                       // w20
    bool v58;                       // w22
    float v59;                      // s8
    float v60;                      // s0
    float v61;                      // s2
    float mTurnAngleLimit;          // s1
    float v63;                      // s0
    float v64;                      // s0
    float mTurnAngleFastLimit;      // s1
    int mTurnAccelFrameFast;        // s2
    float v67;                      // s10
    const float* p_y;               // x20
    float v69;                      // s11
    const float* p_z;               // x21
    float v71;                      // s12
    float v72;                      // s8
    float v73;                      // w22
    float v74;                      // w23
    float v75;                      // w24
    float v76;                      // s9
    float v77;                      // s0
    float v78;                      // s4
    float v79;                      // s5
    float v80;                      // s0
    float v81;                      // s6
    float v82;                      // s16
    float v83;                      // s3
    float v84;                      // s1
    float v85;                      // s7
    float v86;                      // s17
    float v87;                      // s5
    float v88;                      // s1
    float v89;                      // s1
    float v90;                      // s2
    float v91;                      // s0
    float mTurnAngleStart;          // s1
    float v93;                      // s0
    const sead::Vector3f* v94;      // [xsp+8h] [xbp-108h]
    sead::Quatf v95;                // [xsp+10h] [xbp-100h] BYREF
    sead::Vector3f v96;             // [xsp+20h] [xbp-F0h] BYREF
    sead::Vector3f v97;             // [xsp+30h] [xbp-E0h] BYREF
    sead::Vector3f v98;             // [xsp+40h] [xbp-D0h] BYREF
    sead::Vector3f a3a;             // [xsp+50h] [xbp-C0h] BYREF
    sead::Vector3f a1;              // [xsp+60h] [xbp-B0h] BYREF
    sead::Vector3f up;              // [xsp+70h] [xbp-A0h] BYREF
    sead::Vector3f front;           // [xsp+80h] [xbp-90h] BYREF
    sead::Vector3f* v103;           // [xsp+98h] [xbp-78h]

    front.x = 0.0;
    front.y = 0.0;
    front.z = 0.0;
    al::calcFrontDir(&front, this->mActor);
    up.x = 0.0;
    up.y = 0.0;
    up.z = 0.0;
    al::calcUpDir(&up, this->mActor);
    al::alongVectorNormalH(&front, front, up, a3);
    al::tryNormalizeOrZero(&front);
    this->_50 = front;
    p_c = &this->_c;
    a1.x = 0.0;
    a1.y = 0.0;
    a1.z = 0.0;
    if (!al::isNearZero(this->_c, 0.001) || !al::isNearZero(this->_24, 0.001)) {
        al::alongVectorNormalH(&a1, this->_c, this->_24, a3);
        al::tryNormalizeOrZero(&a1);
    }

    v94 = &a3;
    Gravity = &al::getGravity(this->mActor);
    x = Gravity->x;
    y = Gravity->y;
    z = Gravity->z;
    a3a.x = 0.0;
    a3a.y = 0.0;
    v11 = -x;
    a3a.z = 0.0;
    v12 = al::tryNormalizeOrZero(&a3a, a2);
    v13 = 0;
    if ((float)((float)((float)(up.x * v11) - (float)(up.y * y)) - (float)(up.z * z)) < 0.087156)
        v13 = this->_89;

    v14 = al::isNearZero(front, 0.001);
    v15 = v13 || v14;
    v16 = al::isNearZero(a1, 0.001);
    v17 = v12 && !v16;
    v18 = v16 || !v12;
    v103 = (sead::Vector3f*)&a2;
    if (v18 || (float)((float)((float)(a3a.x * a1.x) + (float)(a3a.y * a1.y)) +
                       (float)(a3a.z * a1.z)) < 0.34202) {
        this->_44 = 0;
        this->_48 = 0;
        v19 = 0.0;
        this->_30 = 0.0;
    } else {
        if (v12 && !v16) {
            v20 = al::calcAngleDegree(a1, a3a);
            if (v20 <= 0.0)
                v20 = -v20;
        } else {
            v20 = 0.0;
        }

        _48 = this->_48;
        arr_size = this->arr_size;
        _44 = this->_44;
        if (_48 < arr_size)
            goto LABEL_21;

        v24 = _48 - 1;
        if (_48 >= 1) {
            v25 = this->_44;
            if (v25 + 1 < arr_size)
                v26 = v25 + 1;
            else
                v26 = 0;

            _48 = v24;
            this->_44 = v26;
            this->_48 = v24;
            arr_size = this->arr_size;
            _44 = this->_44;
        }

        if (_48 < arr_size) {
        LABEL_21:
            v27 = _48 + 1;
            v28 = _44 + _48;
            if (v28 < arr_size)
                arr_size = 0;

            v29 = v28 - arr_size;
            arr = this->arr;
            this->_48 = v27;
            arr[v29] = v20;
        }

        v31 = this->_48;
        this->_30 = 0.0;
        if (v31 <= 5)
            v32 = v31;
        else
            v32 = 5;

        v33 = v31 - 1;  //__OFSUB__
        v34 = v31 - 1;
        if (v34 < 0 != v33) {
            v38 = 0.0;
        } else {
            v35 = this->arr_size;
            v36 = this->_44;
            v37 = this->arr;
            v38 = 0.0;
            v39 = 0;
            do {
                if (v34 + v36 >= v35)
                    v40 = v35;
                else
                    v40 = 0;

                v41 = v34 + v36 - v40;
                --v34;
                v38 = v37[v41] + v38;
                ++v39;
                this->_30 = v38;
            } while (v39 < v32);
        }

        v19 = v38 / 5.0;
        this->_30 = v38 / 5.0;
    }

    if (((v13 | v14 | v12 ^ 1) & 1) != 0) {
        v42 = 0;
        v43 = 0;
        v44 = v12;
    } else {
        isOppositeDir = PlayerActionFunction::isOppositeDir(a3a, front);
        v42 = 0;
        if (v18 || v17 && v19 <= 1.0) {
            v44 = v12;
            v43 = 0;
            if (isOppositeDir) {
                v43 = this->_88;
                v42 = !this->_88;
                v44 = v12 & v43;
            }
        } else {
            v44 = v12;
            v43 = 0;
        }
    }

    this->_69 = v43;
    this->_68 = v42;
    this->_6a = v13;
    v98.x = 0.0;
    v98.y = 0.0;
    v98.z = 0.0;
    if ((v44 & (v15 ^ 1) & 1) == 0) {
        if (v12) {
            if (v42) {
                v98 = front;
                this->_8 = 0.0;
                goto LABEL_101;
            }

            v98 = a3a;
            if (!v13)
                goto LABEL_101;
        } else {
            if (!v13) {
                if (v15)
                    al::calcFrontDir(&v98, this->mActor);
                else
                    v98 = front;

                this->_24.z = 0.0;
                this->_18.y = 0.0;
                this->_18.z = 0.0;
                this->_24.x = 0.0;
                this->_24.y = 0.0;
                p_c->x = 0.0;
                this->_c.y = 0.0;
                this->_c.z = 0.0;
                this->_18.x = 0.0;
                this->_8 = PlayerActionFunction::brake(this->_8, this->mTurnBrakeFrame,
                                                       this->mTurnAngleLimit);
                goto LABEL_101;
            }

            al::calcFrontDir(&v98, this->mActor);
            if ((float)((float)((float)(up.x * v11) + (float)(up.y * (float)-y)) +
                        (float)(up.z * (float)-z)) < 0.0) {
                v98.x = -v98.x;
                v98.y = -v98.y;
                v98.z = -v98.z;
            }

            this->_24.z = 0.0;
            this->_18.y = 0.0;
            this->_18.z = 0.0;
            this->_24.x = 0.0;
            this->_24.y = 0.0;
            p_c->x = 0.0;
            this->_c.y = 0.0;
            this->_c.z = 0.0;
            this->_18.x = 0.0;
        }

        this->_8 = this->mTurnAngleFastLimit;
        goto LABEL_101;
    }

    if (v17) {
        v46 = (float)(a1.x * a3a.x) + (float)(a1.y * a3a.y);
        v47 = a1.z;
    } else {
        v46 = (float)(front.x * a3a.x) + (float)(front.y * a3a.y);
        v47 = front.z;
    }

    v48 = v46 + (float)(v47 * a3a.z);
    v49 = -1.0;
    if (v48 >= -1.0) {
        v49 = v48;
        if (v48 > 1.0)
            v49 = 1.0;
    }

    v50 = acosf(v49);
    v51 = v103;
    v52 = (float)((float)(front.x * a3a.x) + (float)(front.y * a3a.y)) + (float)(front.z * a3a.z);
    v53 = -1.0;
    if (v52 >= -1.0) {
        v53 =
            (float)((float)(front.x * a3a.x) + (float)(front.y * a3a.y)) + (float)(front.z * a3a.z);
        if (v52 > 1.0)
            v53 = 1.0;
    }

    v54 = acosf(v53);
    v97.x = (float)(a1.y * a3a.z) - (float)(a1.z * a3a.y);
    v97.y = (float)(a1.z * a3a.x) - (float)(a3a.z * a1.x);
    v97.z = (float)(a3a.y * a1.x) - (float)(a1.y * a3a.x);
    al::tryNormalizeOrZero(&v97);
    if (!v17 || al::isNearZero(this->_18, 0.001))
        goto LABEL_61;

    v55 = v50 * 57.296;
    if ((float)((float)((float)(v97.x * this->_18.x) + (float)(v97.y * this->_18.y)) +
                (float)(v97.z * this->_18.z)) <= 0.0) {
        mTurnAngleStart = this->mTurnAngleStart;
        v93 = this->_8;
        if (mTurnAngleStart <= v93)
            mTurnAngleStart = this->_8;

        if (v55 >= mTurnAngleStart) {
            v57 = 0;
            v58 = 0;
            this->_8 = 0.0;
        } else {
            this->_8 = al::converge(v93, 0.0, v55);
            v57 = 0;
            v58 = v55 < this->mTurnAngleStart;
        }

        goto LABEL_62;
    }

    v95.x = (float)(front.y * a3a.z) - (float)(front.z * a3a.y);
    v95.y = (float)(front.z * a3a.x) - (float)(a3a.z * front.x);
    v95.z = (float)(a3a.y * front.x) - (float)(front.y * a3a.x);
    if (!al::tryNormalizeOrZero((sead::Vector3f*)&v95)) {
    LABEL_61:
        v57 = 0;
        v58 = 0;

    LABEL_62:
        v56 = v94;
        goto LABEL_63;
    }

    v56 = &a3;
    if ((float)((float)((float)(v95.x * v97.x) + (float)(v95.y * v97.y)) +
                (float)(v95.z * v97.z)) >= 0.0) {
        v57 = 0;
        v58 = 0;
    } else {
        v57 = v19 > 10.0 || v55 > 3.0;
        v58 = v19 <= 10.0 && v55 <= 3.0;
    }

LABEL_63:
    v59 = v54 * 57.296;
    v60 = al::calcRate01(sqrtf((float)((float)(v51->x * v51->x) + (float)(v51->y * v51->y)) +
                               (float)(v51->z * v51->z)),
                         0.0, 0.8);
    v61 = 0.25;
    if (v60 >= 0.25) {
        v61 = v60;
        if (v60 > 1.0)
            v61 = 1.0;
    }

    mTurnAngleLimit = this->mTurnAngleLimit;
    if (v59 < this->mTurnAngleStart) {
        v63 = this->_8;

    LABEL_68:
        v64 = PlayerActionFunction::brake(v63, this->mTurnBrakeFrame, mTurnAngleLimit);
        goto LABEL_82;
    }

    v63 = this->_8;
    if (v59 >= this->mTurnAngleFast) {
        mTurnAngleFastLimit = this->mTurnAngleFastLimit;
        mTurnAccelFrameFast = this->mTurnAccelFrameFast;
    } else {
        mTurnAngleFastLimit = v61 * mTurnAngleLimit;
        if (v63 >= mTurnAngleFastLimit) {
            mTurnAngleLimit = this->mTurnAngleFastLimit;
            goto LABEL_68;
        }

        mTurnAccelFrameFast = this->mTurnAccelFrame;
    }

    v64 = PlayerActionFunction::accel(v63, mTurnAngleFastLimit,
                                      mTurnAngleFastLimit / (float)mTurnAccelFrameFast);

LABEL_82:
    this->_8 = v64;
    v96.x = 0.0;
    v96.y = 0.0;
    v96.z = 0.0;
    if (v57 || al::isReverseDirection(front, a3a, 0.01)) {
        v67 = v56->x;
        p_y = &v56->y;
        v69 = v56->y;
        p_z = &v56->z;
        v71 = v56->z;
        if ((float)((float)((float)(v97.x * v56->x) + (float)(v97.y * v69)) +
                    (float)(v97.z * v71)) <= 0.0) {
            v67 = -v67;
            v69 = -v69;
            v71 = -v71;
        }

        v72 = (float)(this->_8 * 0.017453) * 0.5;
        v73 = v67;
        v74 = v69;
        v75 = v71;
        v76 = cosf(v72);
        v77 = sinf(v72);
        v78 = v67 * v77;
        v79 = v69 * v77;
        v80 = v71 * v77;
        v81 = (float)((float)(v79 * front.z) - (float)(v80 * front.y)) + (float)(v76 * front.x);
        v82 = (float)(v76 * front.y) + (float)((float)(v80 * front.x) - (float)(front.z * v78));
        v83 = (float)(v76 * front.z) + (float)((float)(v78 * front.y) - (float)(v79 * front.x));
        v84 = (float)((float)-(float)(v78 * front.x) - (float)(v79 * front.y)) -
              (float)(front.z * v80);
        v98.x = (float)((float)(v79 * v83) + (float)((float)(v76 * v81) - (float)(v80 * v82))) -
                (float)(v78 * v84);
        v98.y = (float)((float)((float)(v80 * v81) + (float)(v76 * v82)) - (float)(v78 * v83)) -
                (float)(v79 * v84);
        v98.z = (float)((float)(v76 * v83) + (float)((float)(v78 * v82) - (float)(v79 * v81))) -
                (float)(v80 * v84);
        al::tryNormalizeOrZero(&v98);
        v96.x = v67;
        v96.y = v69;
        v96.z = v71;
    } else {
        v95.x = 0.0;
        v95.y = 0.0;
        v95.z = 0.0;
        v95.w = 1.0;
        al::makeQuatRotationLimit(&v95, front, a3a, this->_8 * 0.017453f);
        v85 =
            (float)((float)(v95.y * front.z) - (float)(v95.z * front.y)) + (float)(v95.w * front.x);
        v86 =
            (float)(front.y * v95.w) + (float)((float)(v95.z * front.x) - (float)(front.z * v95.x));
        v87 =
            (float)(front.z * v95.w) + (float)((float)(front.y * v95.x) - (float)(v95.y * front.x));
        v88 = (float)((float)-(float)(front.x * v95.x) - (float)(v95.y * front.y)) -
              (float)(front.z * v95.z);
        v98.x =
            (float)((float)(v95.y * v87) + (float)((float)(v95.w * v85) - (float)(v95.z * v86))) -
            (float)(v95.x * v88);
        v98.y =
            (float)((float)((float)(v85 * v95.z) + (float)(v86 * v95.w)) - (float)(v87 * v95.x)) -
            (float)(v88 * v95.y);
        v98.z =
            (float)((float)((float)(v86 * v95.x) - (float)(v85 * v95.y)) + (float)(v87 * v95.w)) -
            (float)(v88 * v95.z);
        al::normalize(&v98);
        v96.x = (float)(front.y * a3a.z) - (float)(front.z * a3a.y);
        v96.y = (float)(front.z * a3a.x) - (float)(a3a.z * front.x);
        v96.z = (float)(a3a.y * front.x) - (float)(front.y * a3a.x);
        al::tryNormalizeOrZero(&v96);
        if (v58) {
            v89 = v96.x;
            v90 = v96.y;
            v91 = v96.z;
            if ((float)((float)((float)(v96.x * this->_18.x) + (float)(v96.y * this->_18.y)) +
                        (float)(v96.z * this->_18.z)) < 0.0) {
                v89 = -v96.x;
                v90 = -v96.y;
                v91 = -v96.z;
                v96.x = -v96.x;
                v96.y = -v96.y;
                v96.z = -v96.z;
            }

            v73 = v89;
            v74 = v90;
            v75 = v91;
        } else {
            v73 = v96.x;
            v74 = v96.y;
            v75 = v96.z;
        }

        p_y = &v56->y;
        p_z = &v56->z;
    }

    this->_18.x = v73;
    this->_18.y = v74;
    this->_18.z = v75;
    this->_c = a3a;
    this->_24.x = v56->x;
    this->_24.y = *p_y;
    this->_24.z = *p_z;

LABEL_101:
    this->_5c = v98;
}
void PlayerActionTurnControl::calcTilt(sead::Vector3f*, const sead::Vector3f&, f32){
    CRASH} f32 PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a2) const {
    f32 result = al::easeIn(al::calcRate01(_8, mTurnAngleStart, mTurnAngleLimit));
    if (a2.dot(_18) > 0.0)
        return -result;

    return result;
}
