#include "al/player/PlayerPainPartsKeeper.h"

#include "al/model/PartsModel.h"
#include "al/player/PlayerCostumeInfo.h"
#include "al/player/PlayerModelHolder.h"
#include "al/util/LiveActorUtil.h"
#include "rs/Player.h"

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

        if (mEnableTimer)
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

static sead::Vector3f initialRotation = {0, 270, 180};

void PlayerPainPartsKeeper::createNoseNeedle(const PlayerModelHolder* playerModelHolder,
                                             const al::ActorInitInfo& actorInitInfo) {
    auto* playerFaceActor = playerModelHolder->findModelActor("Normal");
    auto* faceSubActor = al::getSubActor(playerFaceActor, "顔");
    mPlayerFaceActor = playerFaceActor;
    mNeedlesActor = new al::PartsModel("鼻のトゲ");
    mNeedlesActor->initPartsDirect(playerFaceActor, actorInitInfo, "CactusMiniNeedle",
                                   al::getJointMtxPtr(faceSubActor, "Nose"), sead::Vector3f::zero,
                                   initialRotation, {1, 1, 1}, false);
    al::onSyncClippingSubActor(playerFaceActor, mNeedlesActor);
    al::onSyncHideSubActor(playerFaceActor, mNeedlesActor);
    al::onSyncAlphaMaskSubActor(playerFaceActor, mNeedlesActor);
    al::setMaterialProgrammable(mNeedlesActor);
    mNeedlesActor->makeActorDead();
}

void PlayerPainPartsKeeper::appearNeedle() {
    mTimer = 0;
    if (al::isAlive(mNeedlesActor))
        return;

    mNeedlesActor->makeActorAlive();
    const char* actionName;
    if (mPlayerCostumeInfo->isHidePainNose())
        actionName = "NoseOff";
    else
        actionName = "NoseOn";
    al::startAction(mNeedlesActor, actionName);
}
