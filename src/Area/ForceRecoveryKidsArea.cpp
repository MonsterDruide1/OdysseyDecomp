#include "Area/ForceRecoveryKidsArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

ForceRecoveryKidsArea::ForceRecoveryKidsArea(const char* name) : al::AreaObj(name) {}

void ForceRecoveryKidsArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);

    sead::Matrix34f recoveryMtx = sead::Matrix34f::ident;
    al::tryGetLinksMatrixTR(&recoveryMtx, info, "LinkRecoveryPos");

    recoveryMtx.getTranslation(mRecoveryPos);
    recoveryMtx.getBase(mRecoveryUp, 1);
    al::normalize(&mRecoveryUp);
}
