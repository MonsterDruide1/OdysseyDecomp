#include "MapObj/HipDropRepairParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/ItemGenerator.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(HipDropRepairParts, Wait);
NERVE_IMPL(HipDropRepairParts, On);
NERVE_IMPL(HipDropRepairParts, OnWait);

NERVES_MAKE_NOSTRUCT(HipDropRepairParts, OnWait);
NERVES_MAKE_STRUCT(HipDropRepairParts, Wait, On);
}  // namespace

HipDropRepairParts::HipDropRepairParts(const char* name) : al::LiveActor(name) {}

void HipDropRepairParts::init(const al::ActorInitInfo& info) {
    using HipDropRepairPartsFunctor =
        al::FunctorV0M<HipDropRepairParts*, void (HipDropRepairParts::*)()>;

    const char* suffixName = nullptr;
    al::tryGetStringArg(&suffixName, info, "SuffixName");
    al::initMapPartsActor(this, info, suffixName);
    al::initNerve(this, &NrvHipDropRepairParts.Wait, 0);

    mInitTrans = al::getTrans(this);
    al::calcTransLocalOffset(&mItemGeneratePos, this, sead::Vector3f::ey * 100.0f);

    mItemGenerator = new ItemGenerator(this, info);
    mTraceModel = al::tryGetSubActor(this, "跡モデル");
    if (mTraceModel)
        mTraceModel->makeActorDead();

    if (al::isValidStageSwitch(this, "SwitchHide")) {
        al::listenStageSwitchOnOff(this, "SwitchHide",
                                   HipDropRepairPartsFunctor(this, &HipDropRepairParts::startHide),
                                   HipDropRepairPartsFunctor(this, &HipDropRepairParts::startShow));
    } else if (al::isValidStageSwitch(this, "SwitchShow")) {
        al::listenStageSwitchOnOff(this, "SwitchShow",
                                   HipDropRepairPartsFunctor(this, &HipDropRepairParts::startShow),
                                   HipDropRepairPartsFunctor(this, &HipDropRepairParts::startHide));
        al::hideModel(this);
        if (mTraceModel)
            al::hideModel(mTraceModel);
    }

    makeActorAlive();
}

void HipDropRepairParts::startHide() {
    al::hideModelIfShow(this);
    if (mTraceModel)
        al::hideModelIfShow(mTraceModel);
}

void HipDropRepairParts::startShow() {
    al::showModelIfHide(this);
    if (mTraceModel)
        al::showModelIfHide(mTraceModel);
}

bool HipDropRepairParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (rs::isMsgPlayerAndCapHipDropAll(message) &&
        al::isNerve(this, &NrvHipDropRepairParts.Wait)) {
        al::startHitReaction(this, "ヒップドロップ開始");
        al::setNerve(this, &NrvHipDropRepairParts.On);
        if (mTraceModel)
            mTraceModel->makeActorAlive();
    }

    return false;
}

void HipDropRepairParts::exeWait() {}

void HipDropRepairParts::exeOn() {
    f32 moveOffset = (al::getNerveStep(this) + 1) * -25.0f;
    al::setTrans(this, mInitTrans + sead::Vector3f::ey * moveOffset);

    if (al::isGreaterEqualStep(this, 4)) {
        mItemGenerator->generate(mItemGeneratePos, al::getQuat(this));
        al::startHitReaction(this, "ヒップドロップ完了");
        al::tryOnStageSwitch(this, "SwitchHipDropOn");
        al::setNerve(this, &OnWait);
        kill();
    }
}

void HipDropRepairParts::exeOnWait() {}
