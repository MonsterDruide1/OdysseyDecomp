#include "Enemy/EnemyStateRunAway.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(EnemyStateRunAway, Run);
NERVE_IMPL(EnemyStateRunAway, PanicRun);
NERVE_IMPL(EnemyStateRunAway, PanicRunCollided);

NERVE_MAKE(EnemyStateRunAway, Run);
NERVE_MAKE(EnemyStateRunAway, PanicRun);
NERVE_MAKE(EnemyStateRunAway, PanicRunCollided);
}  // namespace

void EnemyStateRunAway::appear() {
    setDead(false);
    if (mParam->mShouldFaceDir)
        mNeedToFaceToDirection = true;
    al::calcFrontDir(&mFrontDir, mActor);
    if (!mScaredOfActor) {
        mFrontDir = sead::Vector3f::ez;
    } else {
        auto& s = al::getTrans(mScaredOfActor);
        auto& t = al::getTrans(mActor);
        sead::Vector3f normalized;
        normalized.x = s.x - t.x;
        normalized.z = s.z - t.z;
        normalized.y = 0.0f;
        al::normalize(&normalized);

        mFrontDir = -normalized;
    }
    al::setNerve(this, &PanicRun);
}
