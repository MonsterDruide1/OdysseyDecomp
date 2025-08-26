#include "Library/Camera/CameraInterpole.h"

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "math/seadQuat.h"

namespace {
using namespace al;
NERVE_IMPL(CameraInterpole, Deactive);
NERVE_IMPL(CameraInterpole, ActiveRotateAxisY);
NERVE_IMPL(CameraInterpole, ActiveHermiteDistanceHV);
NERVE_IMPL(CameraInterpole, ActiveHermite);
// NERVE_IMPL(CameraInterpole, ActiveBrigade);

NERVES_MAKE_STRUCT(CameraInterpole, Deactive, ActiveRotateAxisY, ActiveHermiteDistanceHV,
                   ActiveHermite);
}  // namespace

namespace al {

CameraInterpole::CameraInterpole() : NerveExecutor("カメラ補間") {
    initNerve(&NrvCameraInterpole.Deactive, 0);
}

const f32 interpoleMinDistance = 150.0f;
const f32 interpoleMaxDistance = 2000.0f;
const f32 maxStepsNoEaseOut = 60.0f;
const f32 maxStepsEaseOut = 90.0f;

void CameraInterpole::start(const CameraTicket* ticket, f32 prevFovyDegree, s32 endInterpoleStep) {
    _48.getPos() = _118.getPos();
    _48.getAt() = _118.getAt();
    _48.getUp() = _118.getUp();
    _48.getUp().normalize();

    if (mPrevPoser) {
        mPrevPoser->setField98(false);
        mPrevPoser = nullptr;
    }

    const CameraTicket* prevTicket = mNextTicket;
    mNextTicket = ticket;
    mPrevTicket = prevTicket;
    mPrevFovyDegree = prevFovyDegree;

    if (endInterpoleStep >= 0) {
        mEndInterpoleStep = endInterpoleStep;
    } else if (prevTicket->getPoser()->isEndInterpoleByStep()) {
        mEndInterpoleStep = mPrevTicket->getPoser()->getEndInterpoleStep();
    } else if (ticket->getPoser()->isInterpoleByCameraDistance()) {
        f32 normalizedDistance =
            normalize((_118.getPos() - ticket->getPoser()->getPosition()).length(),
                      interpoleMinDistance, interpoleMaxDistance);
        f32 maxSteps =
            ticket->getPoser()->isInterpoleEaseOut() ? maxStepsEaseOut : maxStepsNoEaseOut;
        s32 interpoleSteps = (s32)lerpValue(30.0f, maxSteps, normalizedDistance);

        sead::Vector3f prevCamDir = {0.0f, 0.0f, 0.0f};
        sead::Vector3f nextCamDir = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcCameraDir(&prevCamDir, mPrevTicket->getPoser());
        alCameraPoserFunction::calcCameraDir(&nextCamDir, ticket->getPoser());
        f32 yDiffDegrees = sead::Mathf::abs(sead::Mathf::rad2deg(sead::Mathf::asin(prevCamDir.y)) -
                                            sead::Mathf::rad2deg(sead::Mathf::asin(nextCamDir.y)));
        s32 yDiffInterpoleSteps = yDiffDegrees < 0.75f ? 0 : (int)(yDiffDegrees / 0.75f);

        mEndInterpoleStep = sead::Mathi::max(interpoleSteps, yDiffInterpoleSteps);
    } else {
        mEndInterpoleStep = ticket->getPoser()->getInterpoleStep();
    }

    if (mEndInterpoleStep < 1 || mIsRequestCancel) {
        mIsRequestCancel = false;
        setNerve(this, &NrvCameraInterpole.Deactive);
        return;
    }

    if (mPrevTicket) {
        if (!isActive() && !alCameraPoserFunction::isChangeTarget(mPrevTicket->getPoser()) &&
            alCameraPoserFunction::isTargetEnableEndAfterInterpole(mPrevTicket->getPoser()) &&
            !alCameraPoserFunction::isInvalidPreCameraEndAfterInterpole(ticket->getPoser())) {
            if (mPrevTicket->getPoser() != ticket->getPoser()) {
                mPrevPoser = mPrevTicket->getPoser();
                mPrevPoser->setField98(true);
            }
        }

        sead::Vector3f x =
            mPrevTicket->getPoser()->getPosition() - mPrevTicket->getPoser()->getTargetTrans();
        sead::Vector3f v45 =
            ticket->getPoser()->getPosition() - ticket->getPoser()->getTargetTrans();
        x.y = 0.0;
        v45.y = 0.0;
        if (tryNormalizeOrZero(&x) && tryNormalizeOrZero(&v45)) {
            f32 v41 = calcAngleDegree(x, v45);
            if (v41 > 105.0f) {
                if (!ticket->getPoser()->isInterpoleByCameraDistance())
                    mEndInterpoleStep =
                        (int)lerpValue(60.0f, 90.0f, normalize(v41, 105.0f, 180.0f));

                setNerve(this, &NrvCameraInterpole.ActiveRotateAxisY);
                return;
            }
        }
    }

    if (isNear(_3c_at, mNextTicket->getPoser()->getTargetTrans(), 250.0f) ||
        (mNextTicket->getPoser()->getTargetTrans() - _3c_at)
                .dot(mNextTicket->getPoser()->getPosition() - _30_pos) >= 0.0f)
        setNerve(this, &NrvCameraInterpole.ActiveHermiteDistanceHV);

    else
        setNerve(this, &NrvCameraInterpole.ActiveHermite);
}

void CameraInterpole::update(const sead::LookAtCamera& camera) {
    _178.getPos() = camera.getPos();
    _178.getAt() = camera.getAt();
    _178.getUp() = camera.getUp();
    _178.getUp().normalize();

    if (mIsRequestCancel) {
        mIsRequestCancel = false;
        if (isActive())
            setNerve(this, &NrvCameraInterpole.Deactive);
    }

    updateNerve();
    if (isActive() && isNear(_30_pos, _3c_at, 0.001f)) {
        _30_pos.set(_178.getPos());
        _3c_at.set(_178.getAt());
        _49_up.set(_178.getUp());
        setNerve(this, &NrvCameraInterpole.Deactive);
    }

    if (isActive() && mNextTicket) {
        if ((mPrevTicket && !alCameraPoserFunction::isInvalidCollider(mPrevTicket->getPoser())) ||
            !alCameraPoserFunction::isInvalidCollider(mNextTicket->getPoser()) ||
            mNextTicket->getPoser()->getPoserFlag()->_c) {
            sead::Vector3f v11v = _30_pos - _3c_at;
            float v14 = v11v.length();
            sead::Vector3f a2 = v11v;
            normalize(&a2);
            sead::Vector3f a3 = _49_up;
            verticalizeVec(&a3, a2, a3);
            if (tryNormalizeOrZero(&a3)) {
                sead::Vector2f v54[5];
                /* = {
                    sead::Vector2f::zero,
                    {-((75.0f / (v14 * sead::Mathf::pi2())) * 360.0f), 0.0f},
                    {(75.0f / (v14 * sead::Mathf::pi2())) * 360.0f, 0.0f},
                    {0.0f, -(15.0f / (v14 * sead::Mathf::pi2())) * 360.0f},
                    {0.0f, (15.0f / (v14 * sead::Mathf::pi2())) * 360.0f},
                };*/
                f32 p75 = (75.0f / (v14 * sead::Mathf::pi2())) * 360.0f;
                f32 p15 = (15.0f / (v14 * sead::Mathf::pi2())) * 360.0f;

                sead::Vector3f a1;
                a1.setCross(a3, a2);
                normalize(&a1);

                // NON_MATCHING: order of loading/storing values
                v54[0] = sead::Vector2f::zero;
                v54[1] = {-p75, 0.0f};
                v54[2] = {p75, 0.0f};
                v54[3] = {0.0f, -p15};
                v54[4] = {0.0f, p15};

                bool v16 = false;
                for (s32 i = 0; i != 5; i++) {
                    sead::Vector3f v53 = v11v;
                    rotateVectorDegree(&v53, v53, a3, v54[i].x);
                    rotateVectorDegree(&v53, v53, a1, v54[i].y);
                    alCameraPoserFunction::CameraCollisionHitResult v52;
                    v52.mCollisionLocation = 3;

                    if (alCameraPoserFunction::checkFirstCameraCollisionArrow(
                            &v52, mNextTicket->getPoser(), _3c_at, v53) &&
                        !v52.mCollisionLocation) {
                        sead::Vector3f v51 = v52.mCollisionHitPos - _30_pos;
                        sead::Vector3f v50 = {0.0f, 0.0f, 0.0f};
                        if (!isNearZero(v51, 0.001f)) {
                            // NON_MATCHING: somehow stores an unused boolean pair per result
                            // first: false;false
                            // second: true;false
                            // third: true;true
                            if (alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), _30_pos, v51) ||
                                alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), _30_pos + a1 * 10.0f,
                                    v51) ||
                                alCameraPoserFunction::checkFirstCameraCollisionArrow(
                                    &v50, 0LL, mNextTicket->getPoser(), _30_pos - a1 * 10.0f,
                                    v51)) {
                                if ((_30_pos - v50).length() < 75.0f) {
                                } else {
                                    continue;
                                }
                            }
                        }

                        sead::Vector3f v47 = v51;
                        float v33 = v51.length();
                        float v34 = v47.length();
                        if (v34 > 0.0)
                            v47 *= (v33 + 75.0f) / v34;

                        parallelizeVec(&v47, a2, v47);
                        float v36 = v47.length();
                        if (_1f0 < v36) {
                            v16 = true;
                            _1f4 = 0.0f;
                            _1f0 = sead::Mathf::min(v36, (v14 + -100.0f));
                        }
                    }
                }
                if (v16) {
                    makeLookAtCamera(&_118);
                    return;
                }
            }
        }
    }

    float v37 = lerpValue(_1f4, 1.0f, 0.3f);
    _1f4 = lerpValue(_1f4, v37, 0.3f);
    // NON_MATCHING: order of loads/argument setup
    f32 v38 = lerpValue(_1f0, 0.0f, _1f4 * 0.3f);
    _1f0 = lerpValue(_1f0, v38, _1f4 * 0.3f);
    float v44 = (_30_pos - _3c_at).length();
    if ((v44 + -100.0f) < _1f0)
        _1f0 = sead::Mathf::clampMax(_1f0, sead::Mathf::max(v44, (v44 + -100.0f)));

    makeLookAtCamera(&_118);
}

bool CameraInterpole::isActive() const {
    return !isNerve(this, &NrvCameraInterpole.Deactive);
}

void CameraInterpole::makeLookAtCamera(sead::LookAtCamera* camera) const {
    sead::Vector3f v5v = {0.0f, 0.0f, 0.0f};

    if (isActive() && _1f0 > 0.1f) {
        v5v.set(_3c_at - _30_pos);
        f32 v8 = v5v.length();
        f32 f10 = _1f0;
        f32 v9 = v5v.length();
        if (v9 > 0.0f)
            v5v *= sead::Mathf::min(v8 + -100.0f, f10) / v9;
    }

    camera->getPos() = _30_pos + v5v;
    camera->getAt() = _3c_at;
    camera->getUp() = _49_up;
    camera->getUp().normalize();
}

void CameraInterpole::lerpFovyDegree(f32 a2) {
    _1d8 = lerpValue(mPrevFovyDegree, mNextTicket->getPoser()->getFovyDegree(), a2);
}

void CameraInterpole::exeDeactive() {
    if (isFirstStep(this)) {
        if (mPrevPoser) {
            mPrevPoser->setField98(false);
            mPrevPoser = nullptr;
        }
        _1f0 = 0.0f;
    }

    _30_pos.set(_178.getPos());
    _3c_at.set(_178.getAt());
    _49_up.set(_178.getUp());
}

void sub_something(sead::Vector3f*, const sead::LookAtCamera&, const sead::LookAtCamera&,
                   sead::Vector3f, sead::Vector3f, f32);

void CameraInterpole::exeActiveHermite() {
    sead::LookAtCamera* cam = &_48;
    if (mPrevPoser) {
        if (alCameraPoserFunction::isChangeTarget(mPrevPoser))
            mPrevPoser = nullptr;
        else {
            mPrevPoser->movement();
            mPrevPoser->calcCameraPose(cam);
        }
    }

    CameraPoser* poser = mNextTicket->getPoser();
    s32 v28 = mEndInterpoleStep;
    f32 v8 = normalize(sead::Mathi::clampMin(getNerveStep(this) + 1, 1), 0, v28);
    f32 v10 = hermiteRate(v8, 1.5f, 0.0f);

    if (poser->isInterpoleEaseOut())
        v10 = easeOut(v10);

    lerpVec(&_30_pos, _48.getPos(), _178.getPos(), v10);
    lerpVec(&_3c_at, _48.getAt(), _178.getAt(), v10);

    sub_something(&_49_up, _48, _178, _30_pos, _3c_at, v10);

    lerpFovyDegree(v10);

    if (isGreaterEqualStep(this, mEndInterpoleStep - 1))
        setNerve(this, &NrvCameraInterpole.Deactive);
}

f32 cameraGetHAngle(const sead::LookAtCamera&);

void sub_something(sead::Vector3f* up, const sead::LookAtCamera& cam1,
                   const sead::LookAtCamera& cam2, sead::Vector3f pos, sead::Vector3f at,
                   f32 interpole) {
    sead::StorageFor<sead::Quatf> quat;
    sead::Vector3f back = at - pos;
    if (tryNormalizeOrZero(&back) && !isParallelDirection(sead::Vector3f::ey, back, 0.01f)) {
        sead::Vector3f side;
        side.setCross(sead::Vector3f::ey, back);
        normalize(&side);
        rotateVectorDegree(up, sead::Vector3f::ey, side,
                           lerpValue(cameraGetHAngle(cam1), cameraGetHAngle(cam2), interpole));
        return;
    }

    if (isNearDirection(cam1.getUp(), cam2.getUp(), 0.01f))
        return;

    quat.construct(sead::Quatf::unit);
    makeQuatRotationRate(quat.data(), cam1.getUp(), cam2.getUp(), interpole);
    up->setRotated(*quat.data(), cam1.getUp());
    normalize(up);
}

const sead::Vector3f stru_something = {0.0f, 1.0f, 0.0f};

void CameraInterpole::exeActiveHermiteDistanceHV() {
    sead::LookAtCamera* cam = &_48;
    if (mPrevPoser) {
        if (alCameraPoserFunction::isChangeTarget(mPrevPoser))
            mPrevPoser = nullptr;
        else {
            mPrevPoser->movement();
            mPrevPoser->calcCameraPose(cam);
        }
    }

    CameraPoser* poser1 = mNextTicket->getPoser();
    s32 v28_ = mEndInterpoleStep;
    f32 v7 = normalize(sead::Mathi::clampMin(getNerveStep(this), 1), 0, v28_);
    f32 v9 = hermiteRate(v7, 1.5f, 0.0f);

    if (poser1->isInterpoleEaseOut())
        v9 = easeOut(v9);

    CameraPoser* poser2 = mNextTicket->getPoser();
    s32 v28_2 = mEndInterpoleStep;
    f32 v14 = normalize(sead::Mathi::clampMin(getNerveStep(this) + 1, 1), 0, v28_2);
    f32 v16 = hermiteRate(v14, 1.5f, 0.0f);

    if (poser2->isInterpoleEaseOut())
        v16 = easeOut(v16);

    sead::Vector3f a3 = _30_pos - _3c_at;
    verticalizeVec(&a3, stru_something, a3);
    if (!tryNormalizeOrZero(&a3)) {
        setNerve(this, &NrvCameraInterpole.Deactive);
        return;
    }

    lerpVec(&_3c_at, _48.getAt(), _178.getAt(), v16);

    sead::Vector3f x = _48.getPos() - _48.getAt();
    sead::Vector3f a1 = _178.getPos() - _178.getAt();

    f32 v25 = lerpValue(x.length(), a1.length(), v16);

    tryNormalizeOrZero(&a1);
    if (!tryNormalizeOrZero(&x))
        x.set(a1);

    f32 v28 = lerpValue(sead::Mathf::rad2deg(sead::Mathf::asin(x.y)),
                        sead::Mathf::rad2deg(sead::Mathf::asin(a1.y)), v16);

    verticalizeVec(&a1, stru_something, a1);
    tryNormalizeOrZero(&a1);
    if (!isNormalize(a1, 0.001f)) {
        setNerve(this, &NrvCameraInterpole.Deactive);
        return;
    }

    sead::Vector3f v49 = {0.0f, 0.0f, 0.0f};
    turnVecToVecRate(&v49, a3, a1, normalize(v16 - v9, 0.0f, 1.0f - v9));
    normalize(&v49);

    sead::Vector3f v333435 = sead::Mathf::cos(sead::Mathf::deg2rad(v28)) * v49;
    v49 = v333435 + sead::Mathf::sin(sead::Mathf::deg2rad(v28)) * sead::Vector3f{0.0f, 1.0f, 0.0f};
    normalize(&v49);

    _30_pos = (v25 * v49) + _3c_at;

    // NON_MATCHING: Stores reference to _178 in local variable
    sub_something(&_49_up, _48, _178, _30_pos, _3c_at, v16);

    lerpFovyDegree(v16);

    if (isGreaterEqualStep(this, mEndInterpoleStep - 1))
        setNerve(this, &NrvCameraInterpole.Deactive);
}

void CameraInterpole::exeActiveRotateAxisY() {
    sead::LookAtCamera* cam = &_48;
    if (mPrevPoser) {
        if (alCameraPoserFunction::isChangeTarget(mPrevPoser))
            mPrevPoser = nullptr;
        else {
            mPrevPoser->movement();
            mPrevPoser->calcCameraPose(cam);
        }
    }

    CameraPoser* poser = mNextTicket->getPoser();
    s32 v28_ = mEndInterpoleStep;
    f32 v8 = normalize(sead::Mathi::clampMin(getNerveStep(this) + 1, 1), 0, v28_);
    f32 v10 = hermiteRate(v8, 1.5f, 0.0f);

    if (poser->isInterpoleEaseOut())
        v10 = easeOut(v10);

    lerpVec(&_3c_at, _48.getAt(), _178.getAt(), v10);

    sead::Vector3f v47 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f a3 = _48.getPos() - _48.getAt();
    normalize(&a3);
    v47.set(a3);
    verticalizeVec(&a1, sead::Vector3f::ey, a3);
    normalize(&a1);

    sead::Vector3f v45 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f v44 = {0.0f, 0.0f, 0.0f};
    a3 = _178.getPos() - _178.getAt();
    normalize(&a3);
    v45.set(a3);
    verticalizeVec(&v44, sead::Vector3f::ey, a3);
    normalize(&v44);

    if (isFirstStep(this)) {
        mAngleRotateY = 0.0f;
        _1e4.set(a1);
    }

    mAngleRotateY += calcAngleOnPlaneDegree(_1e4, v44, sead::Vector3f::ey);
    a3.set(a1);
    rotateVectorDegreeY(&a3, mAngleRotateY);

    mAngleRotateY += calcAngleOnPlaneDegree(a3, v44, sead::Vector3f::ey);
    a3.set(a1);
    rotateVectorDegreeY(&a3, v10 * mAngleRotateY);

    _1e4.set(v44);

    sead::Vector3f v43 = a3.cross(sead::Vector3f::ey);
    f32 v18 = calcAngleOnPlaneDegree(a1, v47, a1.cross(sead::Vector3f::ey));
    f32 v19 = calcAngleOnPlaneDegree(v44, v45, v44.cross(sead::Vector3f::ey));
    f32 v20 = lerpValue(v18, v19, v10);
    sead::Vector3f v42;
    v42.set(a3);
    rotateVectorDegree(&v42, v42, v43, v20);

    f32 v28 = lerpValue((_48.getPos() - _3c_at).length(), (_178.getPos() - _3c_at).length(), v10);
    _30_pos.setScaleAdd(v28, v42, _3c_at);

    sub_something(&_49_up, _48, _178, _30_pos, _3c_at, v10);

    lerpFovyDegree(v10);

    if (isGreaterEqualStep(this, mEndInterpoleStep - 1))
        setNerve(this, &NrvCameraInterpole.Deactive);
}

void CameraInterpole::exeActiveBrigade() {
    if (mPrevPoser) {
        if (alCameraPoserFunction::isChangeTarget(mPrevPoser))
            mPrevPoser = nullptr;
        else {
            sead::LookAtCamera* cam = &_48;
            mPrevPoser->movement();
            mPrevPoser->calcCameraPose(cam);
        }
    }

    s32 v28 = mEndInterpoleStep;
    f32 v7 = normalize(sead::Mathi::clampMin(getNerveStep(this) + 1, 1), 0, v28);
    f32 v9 = easeInOut(v7);

    lerpVec(&_30_pos, _48.getPos(), _178.getPos(), v9);
    lerpVec(&_3c_at, _48.getAt(), _178.getAt(), v9);

    f32 v10 = calcAngleDegree(_48.getUp(), _178.getUp());
    if (isReverseDirection(_48.getUp(), _178.getUp(), 0.01f)) {
        sead::Vector3f v19 = {0.0f, 0.0f, 0.0f};
        calcDirVerticalAny(&v19, _48.getUp());
        sead::Vector3f v17 = _48.getUp();
        rotateVectorDegree(&v17, v17, v19, v9 * v10);
        _49_up.set(v17);
    } else {
        turnVecToVecDegree(&_49_up, _48.getUp(), _178.getUp(), v9 * v10);
    }

    lerpFovyDegree(v9);

    if (isGreaterEqualStep(this, mEndInterpoleStep - 1))
        setNerve(this, &NrvCameraInterpole.Deactive);
}

f32 cameraGetHAngle(const sead::LookAtCamera& camera) {
    // back = -front
    sead::Vector3f back = camera.getAt() - camera.getPos();
    if (!tryNormalizeOrZero(&back))
        return 0.0f;

    sead::Vector3f side = camera.getUp().cross(back);
    normalize(&side);
    return calcAngleOnPlaneDegree(sead::Vector3f::ey, camera.getUp(), side);
}

}  // namespace al
