#include "Enemy/EnemyStateRunAway.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(EnemyStateRunAway, Run);
NERVE_IMPL(EnemyStateRunAway, PanicRun);
NERVE_IMPL(EnemyStateRunAway, PanicRunCollided);

NERVES_MAKE_NOSTRUCT(EnemyStateRunAway, Run, PanicRun, PanicRunCollided);
}  // namespace

EnemyStateRunAway::EnemyStateRunAway(al::LiveActor* actor, const ParamEnemyStateRunAway* param,
                                     const char* animName)
    : al::ActorStateBase("逃走状態", actor), mParam(param), mAnimName(animName) {
    initNerve(&Run, 0);
}

void EnemyStateRunAway::appear() {
    setDead(false);
    if (mParam->mShouldFaceDir)
        mIsNeedToFaceToDirection = true;
    al::calcFrontDir(&mFrontDir, mActor);
    calcRunDirBase(&mFrontDir);
    al::setNerve(this, &PanicRun);
}

void EnemyStateRunAway::calcRunDirBase(sead::Vector3f* direction) {
    if (!mScaredOfActor) {
        *direction = sead::Vector3f::ez;
    } else {
        const auto& s = al::getTrans(mScaredOfActor);
        const auto& t = al::getTrans(mActor);
        sead::Vector3f normalized;
        normalized.x = s.x - t.x;
        normalized.z = s.z - t.z;
        normalized.y = 0.0f;
        al::normalize(&normalized);

        *direction = -normalized;
    }
}

void EnemyStateRunAway::exeRun() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(mActor, mAnimName);
        al::calcFrontDir(&mFrontDir, mActor);
    }
    sead::Vector3f testDir = mFrontDir;
    calcRunDirBase(&mFrontDir);
    mIsCollidedWallVelocity = al::isCollidedWallVelocity(mActor);
    if (mIsCollidedWallVelocity) {
        sead::Vector3f collidedWallNormal = al::getCollidedWallNormal(mActor);
        al::verticalizeVec(&collidedWallNormal, al::getGravity(mActor), collidedWallNormal);
        if (al::tryNormalizeOrZero(&collidedWallNormal)) {
            al::verticalizeVec(&testDir, collidedWallNormal, testDir);
            if (!al::tryNormalizeOrZero(&testDir)) {
                sead::Vector3f upDir;
                al::calcUpDir(&upDir, mActor);
                mFrontDir.setCross(upDir, collidedWallNormal);
            } else {
                mFrontDir.set(testDir);
            }
            al::normalize(&mFrontDir, mFrontDir);
        }
    }
    al::walkAndTurnToDirection(mActor, mFrontDir, mParam->field_0, mParam->field_c,
                               mParam->field_10, mParam->field_4, true);
}

void EnemyStateRunAway::exePanicRun() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(mActor, mAnimName);
    if ((al::getNerveStep(this) % 180) == 0) {
        calcRunDirBase(&mFrontDir);
        if (mIsNeedToFaceToDirection) {
            al::faceToDirection(mActor, mFrontDir);
            mIsNeedToFaceToDirection = false;
        } else {
            al::rotateVectorDegreeY(&mFrontDir, (al::getRandom() * 60.0f) + -30.0f);
        }
    }
    if (al::isCollidedWallVelocity(mActor)) {
        sead::Vector3f collidedWallNormal = al::getCollidedWallNormal(mActor);
        al::verticalizeVec(&collidedWallNormal, al::getGravity(mActor), collidedWallNormal);
        if (!al::tryNormalizeOrZero(&collidedWallNormal))
            mFrontDir.set(-mFrontDir);
        else
            mFrontDir.set(collidedWallNormal);
        al::setNerve(this, &PanicRunCollided);
    } else {
        al::walkAndTurnToDirection(mActor, mFrontDir, mParam->field_0, mParam->field_c,
                                   mParam->field_10, mParam->field_4, true);
    }
}

void EnemyStateRunAway::exePanicRunCollided() {
    al::walkAndTurnToDirection(mActor, mFrontDir, mParam->field_0, mParam->field_c,
                               mParam->field_10, mParam->field_8, true);
    if (al::isGreaterEqualStep(this, 180))
        al::setNerve(this, &PanicRun);
}
