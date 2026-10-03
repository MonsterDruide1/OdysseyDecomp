#include "Player/PlayerWetControl.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"

#include "Player/PlayerAreaChecker.h"
#include "Player/PlayerEffect.h"

constexpr s32 cWetDuration = 480;

PlayerWetControl::PlayerWetControl(const al::LiveActor* player, al::LiveActor* model,
                                   const PlayerAreaChecker* areaChecker)
    : mPlayer(player), mModel(model), mAreaChecker(areaChecker) {
    al::setMaterialProgrammable(model);
}

bool PlayerWetControl::isWet() const {
    return mWetCounter > 179 || mIsInWetArea;
}

void PlayerWetControl::reset() {
    mInWaterCounter = -1;
    mWetCounter = -1;
    mRoughness = 1.0f;
    mIsInWetArea = mAreaChecker->isInWet(al::getTrans(mPlayer));
    if (mEffect)
        mEffect->tryDeleteWetEffect();
    if (!mIsInWetArea)
        updateModelRoughness(1.0f);
}

void PlayerWetControl::updateModelRoughness(f32 roughness) {
    s32 subActorCount = al::getSubActorNum(mModel);
    al::LiveActor* model = mModel;
    s32 materialCount = al::getMaterialCount(model);
    for (s32 i = 0; i < materialCount; i++)
        al::setModelMaterialParameterF32(model, al::getMaterialName(model, i), "force_roughness",
                                         roughness);

    for (s32 i = 0; i < subActorCount; i++) {
        al::LiveActor* subActor = al::getSubActor(mModel, i);
        s32 subMaterialCount = al::getMaterialCount(subActor);
        for (s32 j = 0; j < subMaterialCount; j++)
            al::setModelMaterialParameterF32(subActor, al::getMaterialName(subActor, j),
                                             "force_roughness", roughness);
    }
}

void PlayerWetControl::recordInWater() {
    mInWaterCounter = 20;
    if (mEffect)
        mEffect->tryDeleteWetEffect();
    mWetCounter = cWetDuration;
}

void PlayerWetControl::recordWet() {
    mWetCounter = cWetDuration;
}

void PlayerWetControl::recordWaterSurface() {
    mIsWaterSurface = true;
    if (mEffect)
        mEffect->tryDeleteWetEffect();
    mWetCounter = al::converge(mWetCounter, cWetDuration, 10);
}

void PlayerWetControl::recordHeavyLandPuddle() {
    mWetCounter = cWetDuration;
}

void PlayerWetControl::recordPuddleRolling() {
    mWetCounter = al::converge(mWetCounter, cWetDuration, 10);
}

void PlayerWetControl::recordForestWaterFall() {
    mWetCounter = al::converge(mWetCounter, cWetDuration, 10);
}

void PlayerWetControl::recordWaterSplash() {
    mWetCounter = al::converge(mWetCounter, cWetDuration, 2);
}

void PlayerWetControl::recordWetBySensor() {
    if (mEffect)
        mEffect->tryDeleteWetEffect();
    mWetCounter = cWetDuration;
}

void PlayerWetControl::update() {
    if (mAreaChecker->isInWet(al::getTrans(mPlayer))) {
        if (mRoughness < 1.0f) {
            reset();
            updateModelRoughness(1.0f);
        }
        mIsInWetArea = true;
        return;
    }

    if (mIsInWetArea)
        mIsInWetArea = false;

    if (mInWaterCounter < 0 && mWetCounter >= cWetDuration - 1 && mEffect && !mIsWaterSurface)
        mEffect->tryEmitWetEffect();
    mIsWaterSurface = false;

    if (mInWaterCounter >= 0) {
        f32 rate = al::calcRate01(mInWaterCounter, 0.0f, 20.0f);
        f32 target = al::lerpValue(0.25f, 0.6f, rate);
        if (mRoughness > 0.6f)
            target = al::converge(mRoughness, 0.6f, 0.05f);
        else if (!al::isNearZeroOrGreater(mRoughness - target, 0.001f))
            target = al::converge(mRoughness, target, 0.05f);
        mRoughness = target;
        updateModelRoughness(target);
        mInWaterCounter--;
        return;
    }

    if (mWetCounter >= 0) {
        f32 rate = al::calcRate01(mWetCounter, 0.0f, 300.0f);
        rate = al::easeOut(rate);
        rate = al::easeOut(rate);
        mRoughness = al::lerpValue(1.0f, 0.25f, rate);
        updateModelRoughness(mRoughness);
        mWetCounter--;
    }
}
