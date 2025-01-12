#include "Player/PlayerPainPartsKeeper.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Obj/PartsModel.h"

#include "Player/Player.h"
#include "Player/PlayerCostumeInfo.h"
#include "Player/PlayerModelHolder.h"

PlayerPainPartsKeeper::PlayerPainPartsKeeper(const al::LiveActor* liveActor,
                                             const PlayerCostumeInfo* costumeInfo)
    : mLiveActor(liveActor), mPlayerCostumeInfo(costumeInfo) {}

void PlayerPainPartsKeeper::update() {
    updateNeedle();
}

void PlayerPainPartsKeeper::updateNeedle() {
    if (!mNeedlesActor || al::isDead(mNeedlesActor))
        return;

    if (!mPlayerCostumeInfo->isPreventHeadPain() || !rs::isGuardNosePainCap(mLiveActor)) {
        al::setModelAlphaMask(mNeedlesActor, mModelAlphaMask);
        if (al::isHideModel(mPlayerFaceActor))
            al::hideModelIfShow(mNeedlesActor);
        else
            al::showModelIfHide(mNeedlesActor);

        if (mIsEnableTimer)
            mTimer++;
        if (mTimer < 3600)
            return;
    }

    mNeedlesActor->kill();
}

void PlayerPainPartsKeeper::resetPosition() {
    if (mNeedlesActor && al::isAlive(mNeedlesActor))
        al::resetPosition(mNeedlesActor);
}

bool PlayerPainPartsKeeper::isEnableNosePain() const {
    if (mPlayerCostumeInfo->isPreventHeadPain() && rs::isGuardNosePainCap(mLiveActor))
        return false;
    return true;
}

bool PlayerPainPartsKeeper::isInvalidNoseDynamics() const {
    return mNeedlesActor && al::isAlive(mNeedlesActor);
}

static sead::Vector3f initialPosition = {0, 0, 0};
static sead::Vector3f initialRotation = {0, 270, 180};

void PlayerPainPartsKeeper::createNoseNeedle(const PlayerModelHolder* playerModelHolder,
                                             const al::ActorInitInfo& actorInitInfo) {
    auto* playerFaceActor = playerModelHolder->findModelActor("Normal");
    auto* faceSubActor = al::getSubActor(playerFaceActor, "顔");
    mPlayerFaceActor = playerFaceActor;
    mNeedlesActor = new al::PartsModel("鼻のトゲ");
    mNeedlesActor->initPartsDirect(playerFaceActor, actorInitInfo, "CactusMiniNeedle",
                                   al::getJointMtxPtr(faceSubActor, "Nose"), initialPosition,
                                   initialRotation, {1.0f, 1.0f, 1.0f}, false);
    al::onSyncClippingSubActor(playerFaceActor, mNeedlesActor);
    al::onSyncHideSubActor(playerFaceActor, mNeedlesActor);
    al::onSyncAlphaMaskSubActor(playerFaceActor, mNeedlesActor);
    al::setMaterialProgrammable(mNeedlesActor);
    mNeedlesActor->makeActorDead();
}

// NON_MATCHING

void PlayerPainPartsKeeper::appearNeedle() {
    mTimer = 0;
    if (al::isAlive(mNeedlesActor))
        return;

    mNeedlesActor->makeActorAlive();
    if (mPlayerCostumeInfo->isHidePainNose())
        al::startAction(mNeedlesActor, "NoseOff");
    else
        al::startAction(mNeedlesActor, "NoseOn");
}
