#include "MapObj/MeganeMapParts.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(MeganeMapParts, Hide);
NERVE_IMPL(MeganeMapParts, Show);
NERVE_IMPL(MeganeMapParts, SwitchAppear);
NERVE_IMPL(MeganeMapParts, On);
NERVE_IMPL(MeganeMapParts, Off);

NERVES_MAKE_STRUCT(MeganeMapParts, Hide, Show, SwitchAppear, On, Off);
}  // namespace

MeganeMapParts::MeganeMapParts(const char* name) : al::LiveActor(name) {}

void MeganeMapParts::init(const al::ActorInitInfo& info) {
    const char* modelName = nullptr;
    if (alPlacementFunction::tryGetModelName(&modelName, info))
        al::initActorWithArchiveName(this, info, modelName, nullptr);
    else
        al::initActor(this, info);

    al::initNerve(this, &NrvMeganeMapParts.Hide, 0);
    al::setMaterialProgrammable(this);
    makeActorAlive();
    al::hideModelIfShow(this);
}

bool MeganeMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isMsgRestart(message)) {
        appear();
        al::showModelIfHide(this);
        al::invalidateClipping(this);

        mAlpha = 1.0f;
        updateAlpha();

        if (rs::isPlayerEnableToSeeOddSpace(this))
            al::setNerve(this, &NrvMeganeMapParts.Show);
        else
            al::setNerve(this, &NrvMeganeMapParts.SwitchAppear);

        return true;
    }
    return false;
}

void MeganeMapParts::control() {
    al::setModelMaterialParameterF32(this, al::getMaterialName(this, 0), "const_single0", mAlpha);
}

void MeganeMapParts::updateAlpha() {
    if (!al::isExistSubActorKeeper(this))
        return;

    f32 alpha = sead::Mathf::clamp((mAlpha - (7.0f / 15.0f)) / (8.0f / 15.0f), 0.0f, 1.0f);
    s32 subActorNum = al::getSubActorNum(this);
    for (s32 i = 0; i < subActorNum; ++i)
        al::setModelAlphaMask(al::getSubActor(this, i), alpha);
}

void MeganeMapParts::exeHide() {
    if (al::isFirstStep(this))
        al::hideModelIfShow(this);

    if (rs::isPlayerEnableToSeeOddSpace(this))
        al::setNerve(this, &NrvMeganeMapParts.On);
}

void MeganeMapParts::exeOn() {
    if (al::isFirstStep(this))
        al::showModelIfHide(this);

    mAlpha = sead::Mathf::min(mAlpha + (1.0f / 30.0f), 1.0f);
    if (!rs::isPlayerEnableToSeeOddSpace(this)) {
        al::setNerve(this, &NrvMeganeMapParts.Off);
        return;
    }

    updateAlpha();
    if (mAlpha == 1.0f)
        al::setNerve(this, &NrvMeganeMapParts.Show);
}

void MeganeMapParts::exeShow() {
    if (!rs::isPlayerEnableToSeeOddSpace(this))
        al::setNerve(this, &NrvMeganeMapParts.Off);
}

void MeganeMapParts::exeOff() {
    mAlpha = sead::Mathf::clampMin(mAlpha - (1.0f / 30.0f), 0.0f);
    if (rs::isPlayerEnableToSeeOddSpace(this)) {
        al::setNerve(this, &NrvMeganeMapParts.On);
        return;
    }

    updateAlpha();
    if (mAlpha == 0.0f)
        al::setNerve(this, &NrvMeganeMapParts.Hide);
}

void MeganeMapParts::exeSwitchAppear() {
    mAlpha = sead::Mathf::clampMax((40 - (u32)al::getNerveStep(this)) / 40.0f, 1.0f);
    updateAlpha();

    if (rs::isPlayerEnableToSeeOddSpace(this)) {
        al::setNerve(this, &NrvMeganeMapParts.On);
        return;
    }

    if (al::isGreaterEqualStep(this, 40)) {
        mAlpha = 0.0f;
        al::validateClipping(this);
        al::setNerve(this, &NrvMeganeMapParts.Hide);
    }
}
