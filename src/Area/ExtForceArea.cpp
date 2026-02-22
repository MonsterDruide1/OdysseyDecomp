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
        mSpeedRangeX = 2.0f;
        mSpeedRangeY = -40.0f;
    } else {
        if (!al::isEqualString(forceType, "Weak"))
            return;
        mSpeedRangeX = 0.25f;
        mSpeedRangeY = -40.0f;
    }

    mMagnitude = 0.98f;
}

void ExtForceArea::calcExtForce(sead::Vector3f* pOutForce, const sead::Vector3f& pos,
                                const sead::Vector3f& velocity,
                                const sead::Vector3f& prevVelocity) const {
    pOutForce->x = 0.0f;
    pOutForce->y = 0.0f;
    pOutForce->z = 0.0f;

    if (!isInVolume(pos))
        return;

    sead::Vector3f dirUp = {0.0f, 0.0f, 0.0f};
    al::getAreaObjDirUp(&dirUp, this);

    sead::Vector3f horizPrev = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertPrev = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&horizPrev, &vertPrev, dirUp, prevVelocity);

    sead::Vector3f horizVel = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vertVel = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&horizVel, &vertVel, dirUp, velocity);

    f32 prevVertSpeed = dirUp.dot(vertPrev);
    f32 currVertSpeed = dirUp.dot(vertVel);

    // --- Vertical force ---
    sead::Vector3f vertForce;

    if (al::isSameSign(prevVertSpeed, mSpeedRangeY) &&
        (prevVertSpeed > 0.0f ? prevVertSpeed : -prevVertSpeed) >
            (mSpeedRangeY > 0.0f ? mSpeedRangeY : -mSpeedRangeY)) {
        vertForce = dirUp * (-currVertSpeed);
    } else {
        f32 combinedVert = prevVertSpeed + currVertSpeed;
        f32 targetVert = al::converge(combinedVert, mSpeedRangeY, mSpeedRangeX);
        vertForce = dirUp * (targetVert - combinedVert);
    }

    // --- Horizontal force ---
    sead::Vector3f normalizedHorizPrev = {0.0f, 0.0f, 0.0f};
    sead::Vector3f horizForce;

    if (al::tryNormalizeOrZero(&normalizedHorizPrev, horizPrev)) {
        f32 diff = horizPrev.length() - horizVel.length();
        f32 clamped = sead::Mathf::clampMin(diff, 0.0f);
        f32 forceMag = mMagnitude * clamped;
        horizForce = (normalizedHorizPrev * forceMag) - (horizVel + horizPrev);
    } else {
        horizForce.x = -horizVel.x;
        horizForce.y = -horizVel.y;
        horizForce.z = -horizVel.z;
    }

    pOutForce->x = vertForce.x + horizForce.x;
    pOutForce->y = vertForce.y + horizForce.y;
    pOutForce->z = vertForce.z + horizForce.z;
}
