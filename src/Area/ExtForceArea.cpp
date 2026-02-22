#include "Area/ExtForceArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

ExtForceArea::ExtForceArea(const char* name) : al::AreaObj(name) {}

void ExtForceArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);

    const char* forceType = "Normal";
    if (!al::tryGetStringArg(&forceType, static_cast<const al::PlacementInfo&>(info), "ForceType"))
        return;

    if (al::isEqualString(forceType, "Normal")) {
        mSpeedStep = 2.0f;
        mSpeedTarget = -40.0f;
    } else {
        if (!al::isEqualString(forceType, "Weak"))
            return;
        mSpeedStep = 0.25f;
        mSpeedTarget = -40.0f;
    }

    mMagnitude = 0.98f;
}

void ExtForceArea::calcExtForce(sead::Vector3f* pOutForce, const sead::Vector3f& pos,
                                const sead::Vector3f& velocity,
                                const sead::Vector3f& prevVelocity) const {
    pOutForce->set(0.0f, 0.0f, 0.0f);

    if (!isInVolume(pos))
        return;

    sead::Vector3f dirUp = {0.0f, 0.0f, 0.0f};
    al::getAreaObjDirUp(&dirUp, this);

    sead::Vector3f horizPrevVel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertPrevVel = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&horizPrevVel, &vertPrevVel, dirUp, prevVelocity);

    sead::Vector3f horizVel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertVel = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&horizVel, &vertVel, dirUp, velocity);

    f32 prevVertSpeed = dirUp.dot(vertPrevVel);
    f32 currVertSpeed = dirUp.dot(vertVel);

    // --- Vertical force ---
    sead::Vector3f vertForce;

    if (al::isSameSign(prevVertSpeed, mSpeedTarget) &&
        (prevVertSpeed > 0.0f ? prevVertSpeed : -prevVertSpeed) >
            (mSpeedTarget > 0.0f ? mSpeedTarget : -mSpeedTarget)) {
        vertForce = dirUp * (-currVertSpeed);
    } else {
        f32 combinedVert = prevVertSpeed + currVertSpeed;
        f32 targetVert = al::converge(combinedVert, mSpeedTarget, mSpeedStep);
        vertForce = dirUp * (targetVert - combinedVert);
    }

    // --- Horizontal force ---
    sead::Vector3f normalizedHorizPrevVel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f horizForce;

    if (al::tryNormalizeOrZero(&normalizedHorizPrevVel, horizPrevVel)) {
        f32 diff = horizPrevVel.length() - horizVel.length();
        f32 forceMag = sead::Mathf::clampMin(diff, 0.0f) * mMagnitude;
        horizForce = (normalizedHorizPrevVel * forceMag) - (horizVel + horizPrevVel);
    } else {
        horizForce = -horizVel;
    }

    pOutForce->setAdd(vertForce, horizForce);
}
