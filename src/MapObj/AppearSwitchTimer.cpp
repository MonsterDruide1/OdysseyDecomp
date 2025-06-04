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
                             al::LiveActor* actor) {
    mActor = actor;
    mAudioKeeper = audioKeeper;
    mStageSwitch = stageSwitch;
    mCamera = camera;
    mDemoInfo = al::registDemoRequesterToAddDemoInfo(actor, info, 0);
    initNerve(&NrvAppearSwitchTimer.OffWait, 0);
    al::tryGetArg(&mValidFrame, info, "ValidFrame");
    if (mValidFrame != -1 && rs::isKidsMode(mActor))
        mValidFrame += 298;

    if (al::calcLinkChildNum(info, "TimerValidArea") != 0)
        mLinkAreaObj = al::createLinkArea(info, "TimerValidArea", "TimerAthleticableArea");

    al::tryOffStageSwitch(mStageSwitch, "CameraSwitchOn");
    al::tryOffStageSwitch(mStageSwitch, "CapRackKeepOn");

    ProjectAppearSwitchFactory* switchFactory = new ProjectAppearSwitchFactory();
    s32 childNum = al::calcLinkChildNum(info, "SwitchAppearTarget");
    mActorArray = new sead::PtrArray<al::LiveActor>();
    s32 stepPlaceholder = 50;
    if (childNum >= 1)
        mActorArray->allocBuffer(childNum, nullptr, 8);

    for (s32 i = 0; i < childNum; i++) {
        const char* linkActorName = al::getLinksActorClassName(info, "SwitchAppearTarget", i);
        al::LiveActor* createdActor = nullptr;
        if (!al::isEqualString("Shine", linkActorName)) {
            al::ActorInitInfo info2{};
            al::PlacementInfo placement{};
            al::getLinksInfoByIndex(&placement, *info.placementInfo, "SwitchAppearTarget", i);
            info2.initViewIdSelf(&placement, info);

            const char* className = nullptr;
            al::getClassName(&className, info2);

            al::ActorCreatorFunction creatorFunction = nullptr;
            switchFactory->getEntryIndex(&creatorFunction, className);
            if (creatorFunction != nullptr) {
                const char* displayName;
                al::getDisplayName(&displayName, info2);
                createdActor = creatorFunction(displayName);
                al::initCreateActorWithPlacementInfo(createdActor, info2);
            } else {
                createdActor = al::createLinksActorFromFactory(info, "SwitchAppearTarget", i);
            }

            if (al::isEqualString(className, "TreasureBoxKey"))
                mShine = ((TreasureBoxKey*)createdActor)->getShine();
        } else {
            createdActor = (al::LiveActor*)rs::initLinkShine(info, "SwitchAppearTarget", i);
            mShine = (Shine*)createdActor;
        }

        if (createdActor != nullptr && createdActor->getHitSensorKeeper() != nullptr &&
            createdActor->getHitSensorKeeper()->getSensorNum() != 0) {
            if (al::isEqualString("Shine", linkActorName) ||
                al::isEqualString("TreasureBoxKey", linkActorName)) {
                stepPlaceholder = 150;
            }
            createdActor->makeActorDead();
            mActorArray->pushBack(createdActor);
        }
    }

    if (mShine != nullptr)
        rs::updateHintTrans(mShine, al::getTrans(mActor));

    mIsInvalidHideRouteGuide = al::tryGetBoolArgOrFalse(info, "IsInvalidHideRouteGuide");
    if (!al::tryGetBoolArgOrFalse(info, "IsValidObjectCamera")) {
        stepPlaceholder = 0;
    } else {
        mCameraTicket = al::initDemoObjectCamera(mCamera, info, nullptr, "固定");
        s32 arg = -1;
        if (al::tryGetArg(&arg, info, "DemoCameraFrame") && arg != -1)
            stepPlaceholder = arg;
    }
    mStep = stepPlaceholder;
    al::tryGetArg(&mIsSkipCameraStartInterp, info, "IsSkipCameraStartInterp");
    al::tryGetArg(&mIsSkipCameraEndInterp, info, "IsSkipCameraEndInterp");
    al::tryGetArg(&mCameraStartInterpFrame, info, "CameraStartInterpFrame");
    al::tryGetArg(&mCameraEndInterpFrame, info, "CameraEndInterpFrame");
}

void ShowTimerActors(const sead::PtrArray<al::LiveActor>* actorArray, bool isAthleticDemoStart) {
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
    if (al::isNerve(this, &NrvAppearSwitchTimer.OffWait)) {
        al::tryOnStageSwitch(mStageSwitch, "CameraSwitchOn");
        al::tryOnStageSwitch(mStageSwitch, "CapRackKeepOn");
        if (!mIsInvalidHideRouteGuide)
            rs::offRouteGuideByActor(mActor);

        if (mShine != nullptr)
            rs::updateHintTrans(mShine, al::getTrans(mShine));

        if (mStep != 0) {
            al::setNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoStart);
            updateNerve();
            return;
        }

        al::startSe(mAudioKeeper, "Success");
        if (mValidFrame != -1)
            al::startBgmSituation(mAudioKeeper, "SwitchTimerOn", false, true);
        ShowTimerActors(mActorArray, mStep != 0);
        al::setNerve(this, &NrvAppearSwitchTimer.OnWait);
    }
}

bool AppearSwitchTimer::isSwitchOn() {
    return !al::isNerve(this, &NrvAppearSwitchTimer.OffWait);
}

void AppearSwitchTimer::offSwitch() {
    sead::PtrArray<al::LiveActor>* actorArray = mActorArray;
    s32 size = actorArray->size();
    for (s32 i = 0; i < size; i++) {
        al::LiveActor* actor = actorArray->at(i);
        if (al::isExistModel(actor) && !al::isExistDitherAnimator(actor)) {
            al::LiveActor* actor2 = actorArray->at(i);
            if (!al::sendMsgShowModel(actor2))
                al::showModelIfHide(actor);
        }

        if (!rs::sendMsgKillBySwitchTimer(actor))
            actor->makeActorDead();
    }

    al::tryOffStageSwitch(mStageSwitch, "CameraSwitchOn");
    al::tryOffStageSwitch(mStageSwitch, "CapRackKeepOn");
    if (!mIsInvalidHideRouteGuide)
        rs::onRouteGuideByActor(mActor);

    if (mShine != nullptr)
        rs::updateHintTrans(mShine, al::getTrans(mActor));

    if (al::isNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoCameraInterpoling) ||
        al::isNerve(this, &NrvAppearSwitchTimer.AppearDemo)) {
        al::endCamera(mCamera, mCameraTicket, -1, false);
        rs::requestEndDemoNormal(mActor);
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
    if (rs::requestStartDemoNormal(mActor, false)) {
        if (mIsSkipCameraStartInterp)
            al::startCamera(mCamera, mCameraTicket, 0);
        else
            al::startCamera(mCamera, mCameraTicket, mCameraStartInterpFrame);

        if (mDemoInfo != nullptr)
            al::addDemoActorFromAddDemoInfo(mActor, mDemoInfo);
        al::setNerve(this, &NrvAppearSwitchTimer.WaitAppearDemoCameraInterpoling);
    }
}

void AppearSwitchTimer::exeWaitAppearDemoCameraInterpoling() {
    if (al::isGreaterEqualStep(this, 5) && !al::isActiveCameraInterpole(mCamera, 0))
        al::setNerve(this, &NrvAppearSwitchTimer.AppearDemo);
}

void AppearSwitchTimer::exeAppearDemo() {
    if (al::isFirstStep(this) != 0) {
        ShowTimerActors(mActorArray, mStep != 0);
        al::startSe(mAudioKeeper, "SwitchAppear");
        if (mValidFrame != -1)
            al::startBgmSituation(mAudioKeeper, "SwitchTimerOn", false, true);
    }

    procAppearDitherAnim(0);
    if (al::isGreaterEqualStep(this, mStep)) {
        if (mIsSkipCameraEndInterp)
            al::endCamera(mCamera, mCameraTicket, 0, false);
        else
            al::endCamera(mCamera, mCameraTicket, mCameraEndInterpFrame, false);
        rs::requestEndDemoNormal(mActor);
        al::setNerve(this, &NrvAppearSwitchTimer.OnWait);
        updateNerve();
    }
}

void AppearSwitchTimer::procAppearDitherAnim(s32 offset) {
    s32 nerveStep = al::getNerveStep(this) + offset;
    s32 size = mActorArray->size();

    if (nerveStep == 0) {
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mActorArray->at(i);
            if (!al::sendMsgChangeAlpha(actor, 0.0f) && al::isExistModel(actor)) {
                if (!al::isExistDitherAnimator(actor))
                    al::showModelIfHide(mActorArray->at(i));
                else
                    al::setDitherAnimMaxAlpha(actor, 0.0f);
            }
        }
    } else if (nerveStep < 1) {
        f32 alpha = al::normalize(nerveStep, 0, 1);
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mActorArray->at(i);
            if (!al::sendMsgChangeAlpha(actor, alpha) && al::isExistModel(actor) &&
                al::isExistDitherAnimator(actor)) {
                al::setDitherAnimMaxAlpha(actor, alpha);
            }
        }
    } else if (nerveStep == 1) {
        for (s32 i = 0; i < size; i++) {
            al::LiveActor* actor = mActorArray->at(i);
            if (!al::sendMsgChangeAlpha(actor, 1.0f) && al::isExistModel(actor) &&
                al::isExistDitherAnimator(actor)) {
                al::setDitherAnimMaxAlpha(actor, 1.0f);
            }
        }
    }
}

void AppearSwitchTimer::exeOnWait() {
    procAppearDitherAnim(mStep);
    al::AreaObj* area = mLinkAreaObj;
    if (area != nullptr) {
        al::LiveActor* nearest = al::tryFindNearestPlayerActor(mActor);
        if (nearest != nullptr) {
            if (!al::isInAreaPos(area, al::getTrans(nearest))) {
                offSwitch();
                return;
            }
        }
    }

    if (mValidFrame != -1) {
        al::holdSe(mAudioKeeper, "TimerLoop");
        if (!al::isNerve(this, &NrvAppearSwitchTimer.OnWait) ||
            mValidFrame - al::getNerveStep(this) <= 180) {
            al::stopSe(mAudioKeeper, "TimerLoop", -1, nullptr);
            al::setNerve(this, &NrvAppearSwitchTimer.OnWaitBlink);
        }
    }
}

s32 AppearSwitchTimer::getLastFrame() {
    if (!al::isNerve(this, &NrvAppearSwitchTimer.OnWait))
        return -1;
    else
        return mValidFrame - al::getNerveStep(this);
}

void AppearSwitchTimer::exeOnWaitBlink() {
    if (al::isLessEqualStep(this, 180))
        al::holdSe(mAudioKeeper, "TimerLoopFast");

    al::AreaObj* area = mLinkAreaObj;
    if (area != nullptr) {
        al::LiveActor* nearest = al::tryFindNearestPlayerActor(mActor);
        if (nearest != nullptr && !al::isInAreaPos(area, al::getTrans(nearest))) {
            offSwitch();
            return;
        }
    }

    f32 alpha;
    sead::PtrArray<al::LiveActor>* actorArray = mActorArray;
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
