#include "MapObj/AppearSwitchTimer.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Item/Shine.h"
#include "MapObj/RouteGuideDirector.h"
#include "MapObj/TreasureBoxKey.h"
#include "Scene/ProjectAppearSwitchFactory.h"
#include "System/GameDataUtil.h"
#include "Util/DemoUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(AppearSwitchTimer, WaitAppearDemoStart);
NERVE_IMPL(AppearSwitchTimer, WaitAppearDemoCameraInterpoling);
NERVE_IMPL(AppearSwitchTimer, AppearDemo);
NERVE_IMPL(AppearSwitchTimer, OnWait);
NERVE_IMPL(AppearSwitchTimer, OnWaitBlink);
NERVE_IMPL(AppearSwitchTimer, OffWait);

NERVES_MAKE_STRUCT(AppearSwitchTimer, OffWait, WaitAppearDemoStart, OnWait,
                   WaitAppearDemoCameraInterpoling, AppearDemo, OnWaitBlink);
}  // namespace

AppearSwitchTimer::AppearSwitchTimer() : al::NerveExecutor("AppearSwitchTimer") {}

void AppearSwitchTimer::init(const al::ActorInitInfo& info, const al::IUseAudioKeeper* audioKeeper,
                             al::IUseStageSwitch* stageSwitch, al::IUseCamera* camera,
                             al::LiveActor* host) {
    mHost = host;
    mAudioKeeper = audioKeeper;
    mStageSwitch = stageSwitch;
    mCamera = camera;
    mDemoInfo = al::registDemoRequesterToAddDemoInfo(host, info, 0);
    initNerve(&NrvAppearSwitchTimer.OffWait, 0);
    al::tryGetArg(&mValidFrame, info, "ValidFrame");
    if (mValidFrame != -1 && rs::isKidsMode(mHost))
        mValidFrame += 298;

    if (al::calcLinkChildNum(info, "TimerValidArea") != 0)
        mTimerValidArea = al::createLinkArea(info, "TimerValidArea", "TimerAthleticableArea");

    al::tryOffStageSwitch(mStageSwitch, "CameraSwitchOn");
    al::tryOffStageSwitch(mStageSwitch, "CapRackKeepOn");

    ProjectAppearSwitchFactory* switchFactory = new ProjectAppearSwitchFactory();
    s32 childNum = al::calcLinkChildNum(info, "SwitchAppearTarget");
    mTargetActors = new sead::PtrArray<al::LiveActor>();
    s32 demoCameraFrame = 50;
    if (childNum >= 1)
        mTargetActors->allocBuffer(childNum, nullptr, 8);

    for (s32 i = 0; i < childNum; i++) {
        const char* linkActorName = al::getLinksActorClassName(info, "SwitchAppearTarget", i);
        al::LiveActor* createdActor = nullptr;
        if (al::isEqualString("Shine", linkActorName)) {
            mShine = rs::initLinkShine(info, "SwitchAppearTarget", i);
            createdActor = mShine;
        } else {
            al::ActorInitInfo targetInfo{};
            al::PlacementInfo targetPlacement{};
            al::getLinksInfoByIndex(&targetPlacement, *info.placementInfo, "SwitchAppearTarget", i);
            targetInfo.initViewIdSelf(&targetPlacement, info);

            const char* className = nullptr;
            al::getClassName(&className, targetInfo);

            al::ActorCreatorFunction creatorFunction = nullptr;
            switchFactory->getEntryIndex(&creatorFunction, className);
            if (creatorFunction != nullptr) {
                const char* displayName;
                al::getDisplayName(&displayName, targetInfo);
                createdActor = creatorFunction(displayName);
                al::initCreateActorWithPlacementInfo(createdActor, targetInfo);
            } else {
                createdActor = al::createLinksActorFromFactory(info, "SwitchAppearTarget", i);
            }

            if (al::isEqualString(className, "TreasureBoxKey"))
                mShine = ((TreasureBoxKey*)createdActor)->getShine();
        }

        if (createdActor != nullptr && createdActor->getHitSensorKeeper() != nullptr &&
            createdActor->getHitSensorKeeper()->getSensorNum() != 0) {
            if (al::isEqualString("Shine", linkActorName) ||
                al::isEqualString("TreasureBoxKey", linkActorName)) {
                demoCameraFrame = 150;
            }
            createdActor->makeActorDead();
            mTargetActors->pushBack(createdActor);
        }
    }

    if (mShine != nullptr)
        rs::updateHintTrans(mShine, al::getTrans(mHost));

    mIsInvalidHideRouteGuide = al::tryGetBoolArgOrFalse(info, "IsInvalidHideRouteGuide");
    if (!al::tryGetBoolArgOrFalse(info, "IsValidObjectCamera")) {
        demoCameraFrame = 0;
    } else {
        mCameraTicket = al::initDemoObjectCamera(mCamera, info, nullptr, "固定");
        s32 argDemoCameraFrame = -1;
        if (al::tryGetArg(&argDemoCameraFrame, info, "DemoCameraFrame") && argDemoCameraFrame != -1)
            demoCameraFrame = argDemoCameraFrame;
    }
    mDemoCameraFrame = demoCameraFrame;
    al::tryGetArg(&mIsSkipCameraStartInterp, info, "IsSkipCameraStartInterp");
    al::tryGetArg(&mIsSkipCameraEndInterp, info, "IsSkipCameraEndInterp");
    al::tryGetArg(&mCameraStartInterpFrame, info, "CameraStartInterpFrame");
    al::tryGetArg(&mCameraEndInterpFrame, info, "CameraEndInterpFrame");
}

static void showTimerActors(const sead::PtrArray<al::LiveActor>* actorArray,
                            bool isAthleticDemoStart) {
    s32 size = actorArray->size();
    for (s32 i = 0; i < size; i++) {
        al::LiveActor* actor = actorArray->at(i);
        if (!al::sendMsgRestart(actor))
            actor->appear();

        if (isAthleticDemoStart)
            rs::sendMsgTimerAthleticDemoStart(actor);
        if (!al::sendMsgChangeAlpha(actor, 1.0f) && al::isExistModel(actor) &&
            al::isExistDitherAnimator(actor)) {
            al::setDitherAnimMaxAlpha(actor, 0.0f);
        }
    }
}

void AppearSwitchTimer::onSwitch() {
    if (isSwitchOn())
        return;

    al::tryOnStageSwitch(mStageSwitch, "CameraSwitchOn");
    al::tryOnStageSwitch(mStageSwitch, "CapRackKeepOn");
    if (!mIsInvalidHideRouteGuide)
        rs::offRouteGuideByActor(mHost);

    if (mShine != nullptr)
        rs::updateHintTrans(mShine, al::getTrans(mShine));

    if (mDemoCameraFrame != 0) {
        al::setNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoStart);
        updateNerve();
        return;
    }

    al::startSe(mAudioKeeper, "Success");
    if (mValidFrame != -1)
        al::startBgmSituation(mAudioKeeper, "SwitchTimerOn", false, true);
    showTimerActors(mTargetActors, mDemoCameraFrame != 0);
    al::setNerve(this, &NrvAppearSwitchTimer.OnWait);
}

bool AppearSwitchTimer::isSwitchOn() {
    return !al::isNerve(this, &NrvAppearSwitchTimer.OffWait);
}

void AppearSwitchTimer::offSwitch() {
    sead::PtrArray<al::LiveActor>* actorArray = mTargetActors;
    s32 size = actorArray->size();
    for (s32 i = 0; i < size; i++) {
        al::LiveActor* actor = actorArray->at(i);
        if (al::isExistModel(actor) && !al::isExistDitherAnimator(actor) &&
            !al::sendMsgShowModel(actorArray->at(i))) {
            al::showModelIfHide(actor);
        }

        if (!rs::sendMsgKillBySwitchTimer(actor))
            actor->makeActorDead();
    }

    al::tryOffStageSwitch(mStageSwitch, "CameraSwitchOn");
    al::tryOffStageSwitch(mStageSwitch, "CapRackKeepOn");
    if (!mIsInvalidHideRouteGuide)
        rs::onRouteGuideByActor(mHost);

    if (mShine != nullptr)
        rs::updateHintTrans(mShine, al::getTrans(mHost));

    if (isDemoPlaying()) {
        al::endCamera(mCamera, mCameraTicket, -1, false);
        rs::requestEndDemoNormal(mHost);
    }

    if (mValidFrame != -1)
        al::endBgmSituation(mAudioKeeper, "SwitchTimerOn", false);
    al::setNerve(this, &NrvAppearSwitchTimer.OffWait);
}

bool AppearSwitchTimer::isDemoPlaying() {
    return al::isNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoCameraInterpoling) ||
           al::isNerve(this, &NrvAppearSwitchTimer.AppearDemo);
}

void AppearSwitchTimer::exeWaitAppearDemoStart() {
    if (rs::requestStartDemoNormal(mHost, false)) {
        if (mIsSkipCameraStartInterp)
            al::startCamera(mCamera, mCameraTicket, 0);
        else
            al::startCamera(mCamera, mCameraTicket, mCameraStartInterpFrame);

        if (mDemoInfo != nullptr)
            al::addDemoActorFromAddDemoInfo(mHost, mDemoInfo);
        al::setNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoCameraInterpoling);
    }
}

void AppearSwitchTimer::exeWaitAppearDemoCameraInterpoling() {
    if (al::isGreaterEqualStep(this, 5) && !al::isActiveCameraInterpole(mCamera, 0))
        al::setNerve(this, &NrvAppearSwitchTimer.AppearDemo);
}

void AppearSwitchTimer::exeAppearDemo() {
    if (al::isFirstStep(this)) {
        showTimerActors(mTargetActors, mDemoCameraFrame != 0);
        al::startSe(mAudioKeeper, "SwitchAppear");
        if (mValidFrame != -1)
            al::startBgmSituation(mAudioKeeper, "SwitchTimerOn", false, true);
    }

    procAppearDitherAnim(0);
    if (al::isGreaterEqualStep(this, mDemoCameraFrame)) {
        if (mIsSkipCameraEndInterp)
            al::endCamera(mCamera, mCameraTicket, 0, false);
        else
            al::endCamera(mCamera, mCameraTicket, mCameraEndInterpFrame, false);
        rs::requestEndDemoNormal(mHost);
        al::setNerve(this, &NrvAppearSwitchTimer.OnWait);
        updateNerve();
    }
}

void AppearSwitchTimer::procAppearDitherAnim(s32 offset) {
    s32 nerveStep = al::getNerveStep(this) + offset;
    s32 size = mTargetActors->size();

    if (nerveStep == 0) {
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mTargetActors->at(i);
            if (!al::sendMsgChangeAlpha(actor, 0.0f) && al::isExistModel(actor)) {
                if (!al::isExistDitherAnimator(actor))
                    al::showModelIfHide(mTargetActors->at(i));
                else
                    al::setDitherAnimMaxAlpha(actor, 0.0f);
            }
        }
    } else if (nerveStep < 1) {
        f32 alpha = al::normalize(nerveStep, 0, 1);
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mTargetActors->at(i);
            if (!al::sendMsgChangeAlpha(actor, alpha) && al::isExistModel(actor) &&
                al::isExistDitherAnimator(actor)) {
                al::setDitherAnimMaxAlpha(actor, alpha);
            }
        }
    } else if (nerveStep == 1) {
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mTargetActors->at(i);
            if (!al::sendMsgChangeAlpha(actor, 1.0f) && al::isExistModel(actor) &&
                al::isExistDitherAnimator(actor)) {
                al::setDitherAnimMaxAlpha(actor, 1.0f);
            }
        }
    }
}

void AppearSwitchTimer::exeOnWait() {
    procAppearDitherAnim(mDemoCameraFrame);
    al::AreaObj* area = mTimerValidArea;
    if (area != nullptr) {
        al::LiveActor* player = al::tryFindNearestPlayerActor(mHost);
        if (player && !al::isInAreaPos(area, al::getTrans(player))) {
            offSwitch();
            return;
        }
    }

    if (mValidFrame != -1) {
        al::holdSe(mAudioKeeper, "TimerLoop");
        if (getLastFrame() <= 180) {
            al::stopSe(mAudioKeeper, "TimerLoop", -1, nullptr);
            al::setNerve(this, &NrvAppearSwitchTimer.OnWaitBlink);
        }
    }
}

s32 AppearSwitchTimer::getLastFrame() {
    if (!al::isNerve(this, &NrvAppearSwitchTimer.OnWait))
        return -1;
    return mValidFrame - al::getNerveStep(this);
}

void AppearSwitchTimer::exeOnWaitBlink() {
    if (al::isLessEqualStep(this, 180))
        al::holdSe(mAudioKeeper, "TimerLoopFast");

    al::AreaObj* area = mTimerValidArea;
    if (area != nullptr) {
        al::LiveActor* nearest = al::tryFindNearestPlayerActor(mHost);
        if (nearest != nullptr && !al::isInAreaPos(area, al::getTrans(nearest))) {
            offSwitch();
            return;
        }
    }

    f32 alpha;
    sead::PtrArray<al::LiveActor>* actorArray = mTargetActors;
    s32 nerveStep = al::getNerveStep(this);
    s32 modifiedNerveStep = (nerveStep - 40) % 50;
    s32 size = actorArray->size();

    if (0 < size) {
        if (nerveStep - 180 > 0) {
            for (s32 i = 0; i < size; i++) {
                al::LiveActor* actor = actorArray->at(i);
                alpha = al::normalize(nerveStep - 180, 0, 60);
                if (!al::sendMsgChangeAlpha(actor, 1.0f - alpha) && al::isExistModel(actor) &&
                    al::isExistDitherAnimator(actor)) {
                    al::setDitherAnimMaxAlpha(actor, 1.0f - alpha);
                }
            }
        } else {
            for (s32 i = 0; i < size; i++) {
                al::LiveActor* actor = actorArray->at(i);
                if (modifiedNerveStep < 20) {
                    alpha = (al::normalize(modifiedNerveStep, 0, 5) * -0.502f) + 1.0f;
                    if (!al::sendMsgChangeAlpha(actor, alpha) && al::isExistModel(actor)) {
                        if (!al::isExistDitherAnimator(actor)) {
                            if (modifiedNerveStep == 0 && !al::sendMsgHideModel(actor))
                                al::hideModelIfShow(actor);
                        } else {
                            al::setDitherAnimMaxAlpha(actor, alpha);
                        }
                    }
                } else {
                    alpha = (al::normalize(modifiedNerveStep, 20, 25) * 0.502f) + 0.498f;
                    if (!al::sendMsgChangeAlpha(actor, alpha) && al::isExistModel(actor)) {
                        if (!al::isExistDitherAnimator(actor)) {
                            if (modifiedNerveStep == 20 && !al::sendMsgShowModel(actor))
                                al::showModelIfHide(actor);
                        } else {
                            al::setDitherAnimMaxAlpha(actor, alpha);
                        }
                    }
                }
            }
        }
    }

    if (al::isStep(this, 180))
        al::startSe(mAudioKeeper, "TimeUp");
    if (al::isGreaterEqualStep(this, 240))
        offSwitch();
}

void AppearSwitchTimer::exeOffWait() {}
