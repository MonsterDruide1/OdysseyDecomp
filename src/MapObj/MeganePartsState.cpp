#include "MapObj/MeganePartsState.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(MeganePartsState, Hide);
NERVE_IMPL(MeganePartsState, On);
NERVE_IMPL(MeganePartsState, Off);
NERVE_IMPL(MeganePartsState, Show);

NERVES_MAKE_STRUCT(MeganePartsState, Hide, On, Off, Show);
}  // namespace

MeganePartsState::MeganePartsState(al::LiveActor* actor)
    : al::ActorStateBase("メガネーパーツ状態", actor) {
    initNerve(&NrvMeganePartsState.Hide, 0);
}

void MeganePartsState::appear() {
    al::setMaterialProgrammable(mActor);
    al::hideModelIfShow(mActor);
    al::NerveStateBase::appear();
    al::setNerve(this, &NrvMeganePartsState.Hide);
}

bool MeganePartsState::update() {
    s32 subActorNum = al::getSubActorNum(mActor);
    al::LiveActor* actor = mActor;

    if (subActorNum >= 1) {
        for (s32 i = 0; i != subActorNum; i++) {
            al::LiveActor* subActor = al::getSubActor(actor, i);
            al::setTrans(subActor, al::getTrans(mActor));
            actor = mActor;
        }
    }

    al::setModelMaterialParameterF32(actor, al::getMaterialName(actor, 0), "const_single0", mAlpha);
    return al::NerveStateBase::update();
}

void MeganePartsState::exeHide() {
    if (al::isFirstStep(this))
        al::hideModelIfShow(mActor);

    if (rs::isPlayerEnableToSeeOddSpace(mActor))
        al::setNerve(this, &NrvMeganePartsState.On);
}

void MeganePartsState::exeOn() {
    if (al::isFirstStep(this))
        al::showModelIfHide(mActor);

    mAlpha = sead::Mathf::min(mAlpha + (1.0f / 15.0f), 1.0f);
    if (!rs::isPlayerEnableToSeeOddSpace(mActor)) {
        al::setNerve(this, &NrvMeganePartsState.Off);
        return;
    }

    updateAlpha();
    if (mAlpha == 1.0f)
        al::setNerve(this, &NrvMeganePartsState.Show);
}

void MeganePartsState::updateAlpha() {
    if (!al::isExistSubActorKeeper(mActor))
        return;

    f32 alpha = sead::Mathf::clamp((mAlpha - (7.0f / 15.0f)) / (8.0f / 15.0f), 0.0f, 1.0f);
    s32 subActorNum = al::getSubActorNum(mActor);
    for (s32 i = 0; i < subActorNum; i++)
        al::setModelAlphaMask(al::getSubActor(mActor, i), alpha);
}

void MeganePartsState::exeShow() {
    if (!rs::isPlayerEnableToSeeOddSpace(mActor))
        al::setNerve(this, &NrvMeganePartsState.Off);
}

void MeganePartsState::exeOff() {
    mAlpha = sead::Mathf::clampMin(mAlpha - (1.0f / 15.0f), 0.0f);
    if (rs::isPlayerEnableToSeeOddSpace(mActor)) {
        al::setNerve(this, &NrvMeganePartsState.On);
        return;
    }

    updateAlpha();
    if (mAlpha == 0.0f)
        al::setNerve(this, &NrvMeganePartsState.Hide);
}
