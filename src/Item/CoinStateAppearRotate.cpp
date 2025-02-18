#include "Item/CoinStateAppearRotate.h"

#include <math/seadQuat.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(CoinStateAppearRotate, Rotate);

NERVES_MAKE_STRUCT(CoinStateAppearRotate, Rotate);
}  // namespace

CoinStateAppearRotate::CoinStateAppearRotate(al::LiveActor* actor, al::MtxConnector* mtxConnector,
                                             const sead::Vector3f& displayOffset,
                                             const char* rotateHitReaction)
    : al::ActorStateBase("回転出現状態", actor), mMtxConnector(mtxConnector),
      mDisplayOffset(displayOffset), mRotateHitReaction(rotateHitReaction) {
    initNerve(&NrvCoinStateAppearRotate.Rotate, 0);
}

void CoinStateAppearRotate::appear() {
    setDead(false);
    al::setNerve(this, &NrvCoinStateAppearRotate.Rotate);
}

void CoinStateAppearRotate::exeRotate() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::startHitReaction(actor, mRotateHitReaction);
        mVelocity = sead::Vector3f(0.0f, 22.0f, 0.0f);
        mOffset = sead::Vector3f::zero;
        mInitialTransY = al::getTransPtr(actor)->y;
        al::showModelIfHide(actor);
    }

    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, actor);
    sead::Vector3f upDir = sead::Vector3f::zero;
    al::calcUpDir(&upDir, actor);
    al::rotateVectorDegree(&frontDir, frontDir, upDir, 15.0f);

    if (mMtxConnector == nullptr) {
        al::getTransPtr(actor)->y = mInitialTransY + mOffset.y;

        sead::Quatf quad = sead::Quatf::unit;
        al::makeQuatFrontUp(&quad, frontDir, upDir);
        al::setQuat(actor, quad);

        mVelocity.y -= 1.5f;
        mOffset += mVelocity;
    } else {
        al::connectPoseQT(actor, mMtxConnector);
        *al::getTransPtr(actor) += mDisplayOffset;
        *al::getTransPtr(actor) += mOffset;

        sead::Quatf quad = sead::Quatf::unit;
        al::makeQuatFrontUp(&quad, frontDir, upDir);
        al::setQuat(actor, quad);

        mVelocity.y -= 1.5f;
        mOffset += mVelocity;
    }

    if (mOffset.y < 0.0f) {
        al::getTransPtr(actor)->y = mInitialTransY;
        al::setVelocityZero(actor);
        kill();
    }
}
