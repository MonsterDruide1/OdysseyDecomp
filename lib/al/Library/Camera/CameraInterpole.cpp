#include "Library/Camera/CameraInterpole.h"

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

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

CameraInterpole::~CameraInterpole() {}

const f32 val1 = 150.0f;
const f32 val2 = 2000.0f;
const f32 val3 = 60.0f;
const f32 val4 = 90.0f;

void CameraInterpole::start(const CameraTicket* ticket, f32 a2, int a3) {
    _48.getPos() = _118.getPos();
    _48.getAt() = _118.getAt();
    _48.getUp() = _118.getUp();
    _48.getUp().normalize();

    if (_20) {
        // FIXME
        *(((bool*)_20) + 0x98) = false;
        _20 = nullptr;
    }

    const al::CameraTicket* prevTicket = mNextTicket;
    mNextTicket = ticket;
    mPrevTicket = prevTicket;
    _1dc = a2;

    if (a3 >= 0)
        _28 = a3;
    else if (prevTicket->getPoser()->isEndInterpoleByStep()) {
        _28 = mPrevTicket->getPoser()->getEndInterpoleStep();
    } else if (ticket->getPoser()->isInterpoleByCameraDistance()) {
        f32 v25 =
            al::normalize((_118.getPos() - ticket->getPoser()->getPosition()).length(), val1, val2);
        f32 v27 = ticket->getPoser()->isInterpoleEaseOut() ? val4 : val3;
        int InterpoleStep = (int)al::lerpValue(30.0f, v27, v25);
        sead::Vector3f prevCamDir = {0.0f, 0.0f, 0.0f};
        sead::Vector3f nextCamDir = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcCameraDir(&prevCamDir, mPrevTicket->getPoser());
        alCameraPoserFunction::calcCameraDir(&nextCamDir, ticket->getPoser());
        f32 v29 = sead::Mathf::abs(sead::Mathf::rad2deg(sead::Mathf::asin(prevCamDir.y)) -
                                   sead::Mathf::rad2deg(sead::Mathf::asin(nextCamDir.y)));

        s32 v30 = v29 < 0.75f ? 0 : (int)(v29 / 0.75f);

        if (InterpoleStep <= v30)
            InterpoleStep = v30;
        _28 = InterpoleStep;
    } else {
        _28 = ticket->getPoser()->getInterpoleStep();
    }

    if (_28 < 1 || _2c) {
        _2c = 0;
        al::setNerve(this, &NrvCameraInterpole.Deactive);
        return;
    }

    if (mPrevTicket) {
        if (al::isNerve(this, &NrvCameraInterpole.Deactive) &&
            !alCameraPoserFunction::isChangeTarget(mPrevTicket->getPoser()) &&
            alCameraPoserFunction::isTargetEnableEndAfterInterpole(mPrevTicket->getPoser()) &&
            !alCameraPoserFunction::isInvalidPreCameraEndAfterInterpole(ticket->getPoser())) {
            if (mPrevTicket->getPoser() != ticket->getPoser()) {
                _20 = mPrevTicket->getPoser();
                *(((bool*)_20) + 0x98) = true;
            }
        }

        sead::Vector3f x =
            mPrevTicket->getPoser()->getPosition() - mPrevTicket->getPoser()->getTargetTrans();
        sead::Vector3f v45 =
            ticket->getPoser()->getPosition() - ticket->getPoser()->getTargetTrans();
        x.y = 0.0;
        v45.y = 0.0;
        if (al::tryNormalizeOrZero(&x) && al::tryNormalizeOrZero(&v45)) {
            f32 v41 = al::calcAngleDegree(x, v45);
            if (v41 > 105.0f) {
                if (!ticket->getPoser()->isInterpoleByCameraDistance())
                    _28 = (int)al::lerpValue(60.0f, 90.0f, al::normalize(v41, 105.0f, 180.0f));

                al::setNerve(this, &NrvCameraInterpole.ActiveRotateAxisY);
                return;
            }
        }
    }

    if (al::isNear(_3c_at, mNextTicket->getPoser()->getTargetTrans(), 250.0f) ||
        (mNextTicket->getPoser()->getTargetTrans() - _3c_at)
                .dot(mNextTicket->getPoser()->getPosition() - _30_pos) >= 0.0f)
        al::setNerve(this, &NrvCameraInterpole.ActiveHermiteDistanceHV);

    else
        al::setNerve(this, &NrvCameraInterpole.ActiveHermite);
}

void CameraInterpole::update(const sead::LookAtCamera& camera) {
    _178.getPos() = camera.getPos();
    _178.getAt() = camera.getAt();
    _178.getUp() = camera.getUp();
    _178.getUp().normalize();

    if (_2c) {
        _2c = false;
        if (!al::isNerve(this, &NrvCameraInterpole.Deactive))
            al::setNerve(this, &NrvCameraInterpole.Deactive);
    }

    updateNerve();
    if (!al::isNerve(this, &NrvCameraInterpole.Deactive) && al::isNear(_30_pos, _3c_at, 0.001f)) {
        _30_pos = _178.getPos();
        _3c_at = _178.getAt();
        _49_up = _178.getUp();
        al::setNerve(this, &NrvCameraInterpole.Deactive);
    }

    // TODO
}

bool CameraInterpole::isActive() const {
    // TODO
}

void CameraInterpole::makeLookAtCamera(sead::LookAtCamera* camera) const {
    // TODO
}

void CameraInterpole::lerpFovyDegree(f32) {
    // TODO
}

void CameraInterpole::exeDeactive() {
    // TODO
}

void CameraInterpole::exeActiveHermite() {
    // TODO
}

void CameraInterpole::exeActiveHermiteDistanceHV() {
    // TODO
}

void CameraInterpole::exeActiveRotateAxisY() {
    // TODO
}

void CameraInterpole::exeActiveBrigade() {
    // TODO
}

}  // namespace al
