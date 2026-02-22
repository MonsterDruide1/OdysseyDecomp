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
        mMagnitude = 0.98f;
    } else if (al::isEqualString(forceType, "Weak")) {
        mSpeedStep = 0.25f;
        mSpeedTarget = -40.0f;
        mMagnitude = 0.98f;
    }
}

void ExtForceArea::calcExtForce(sead::Vector3f* force, const sead::Vector3f& pos,
                                const sead::Vector3f& prevForce,
                                const sead::Vector3f& velocity) const {
    force->set(0.0f, 0.0f, 0.0f);

    if (!isInVolume(pos))
        return;

    sead::Vector3f dirUp = {0.0f, 0.0f, 0.0f};
    al::getAreaObjDirUp(&dirUp, this);

    sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&velH, &velV, dirUp, velocity);

    sead::Vector3f prevForceH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f prevForceV = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&prevForceH, &prevForceV, dirUp, prevForce);

    f32 speedV = dirUp.dot(velV);
    f32 prevForceMagnitudeV = dirUp.dot(prevForceV);

    // --- Vertical force ---
    sead::Vector3f vertForce;

    if (al::isSameSign(speedV, mSpeedTarget) &&
        sead::Mathf::abs(speedV) > sead::Mathf::abs(mSpeedTarget)) {
        vertForce = dirUp * (-prevForceMagnitudeV);
    } else {
        f32 combinedVert = speedV + prevForceMagnitudeV;
        f32 targetVert = al::converge(combinedVert, mSpeedTarget, mSpeedStep);
        vertForce = dirUp * (targetVert - combinedVert);
    }

    // --- Horizontal force ---
    sead::Vector3f normalizedvelH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f horizForce;

    if (al::tryNormalizeOrZero(&normalizedvelH, velH)) {
        f32 diff = velH.length() - prevForceH.length();
        f32 forceMag = sead::Mathf::clampMin(diff, 0.0f) * mMagnitude;
        horizForce = (normalizedvelH * forceMag) - (prevForceH + velH);
    } else {
        horizForce = -prevForceH;
    }

    force->setAdd(vertForce, horizForce);
}
