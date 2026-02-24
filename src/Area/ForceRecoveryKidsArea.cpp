#include "Area/ForceRecoveryKidsArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

ForceRecoveryKidsArea::ForceRecoveryKidsArea(const char* name) : al::AreaObj(name) {
    mTargetPos = {0.0f, 0.0f, 0.0f};
}

void ForceRecoveryKidsArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);

    sead::Matrix34f matrix = sead::Matrix34f::ident;
    al::tryGetLinksMatrixTR(&matrix, info, "ForceRecoveryTarget");

    matrix.getTranslation(mTargetPos);
    matrix.getBase(mTargetUp, 1);
    al::normalize(&mTargetUp);
}
