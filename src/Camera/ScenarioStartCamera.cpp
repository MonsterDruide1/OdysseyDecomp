#include "Camera/ScenarioStartCamera.h"

#include <container/seadPtrArray.h>

#include "Camera/ScenarioStartCameraPoser.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Scene/QuestInfo.h"
#include "Scene/QuestInfoHolder.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(ScenarioStartCamera, Play);
NERVE_IMPL(ScenarioStartCamera, PlayAnim);
NERVE_IMPL(ScenarioStartCamera, End);

NERVES_MAKE_NOSTRUCT(ScenarioStartCamera, End);

// __attribute__((used)): the internal-linkage nerve classes and their
// execute()s would otherwise be discarded if nothing referenced the Nrv
// struct. init() references it now (initNerve + setNerve), so the attribute
// is belt-and-braces. End is a standalone variable (not a struct member):
// the original binary lays it out separately from the Play/PlayAnim pair,
// and exePlayAnim takes its address with a single adrp+add, which a struct
// member (+0x10) does not produce.
struct {
    ScenarioStartCameraNrvPlay Play;
    ScenarioStartCameraNrvPlayAnim PlayAnim;
} NrvScenarioStartCamera __attribute__((used));
}  // namespace

ScenarioStartCamera::ScenarioStartCamera(const char* name)
    : al::LiveActor("シナリオ紹介カメラ") {}

void ScenarioStartCamera::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTFSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorWatchObj(this, info);
    al::initStageSwitch(this, info);
    al::initActorBgmKeeper(this, info, nullptr);
    al::initNerve(this, &NrvScenarioStartCamera.Play, 0);
    al::invalidateClipping(this);
    mLinkedShineIndex = GameDataFunction::tryFindLinkedShineIndex(this, info);
    mQuestInfos = rs::tryCreateAndRegisterQuestInfoToHolderFromLinkedObj(this, info);
    if (al::isObjectName(info, "ScenarioStartCameraAnim")) {
        sead::Matrix34f* baseMtx = new sead::Matrix34f(sead::Matrix34f::ident);
        mBaseMtx = baseMtx;
        alActorPoseFunction::calcBaseMtx(baseMtx, this);
        mDemoAnimName = al::getStringArg(info, "CameraAnimName");
        mDemoCameraTicket = al::initDemoAnimCamera(
            this, info, al::findOrCreateResource("ObjectData/ScenarioStartCamera", nullptr),
            getBaseMtx(), mDemoAnimName);
        al::setNerve(this, &NrvScenarioStartCamera.PlayAnim);
    } else {
        mTicketNum = al::calcLinkNestNum(info, "NextKey") + 1;
        mTickets = new al::CameraTicket*[mTicketNum];
        al::PlacementInfo placementInfo = al::getPlacementInfo(info);
        bool isKeep = false;
        al::tryGetArg(&isKeep, info, "IsKeepPreSelfPoseNextCamera");
        for (s64 i = 0; i < mTicketNum; i++) {
            const s64 lastIndex = mTicketNum - 1;
            const char* name = nullptr;
            al::getObjectName(&name, placementInfo);
            ScenarioStartCameraPoser* poser = nullptr;
            if (al::isEqualString(name, "ScenarioStartCameraSimpleZoom")) {
                bool isLast = lastIndex <= i;
                poser = new ScenarioStartCameraPoserSimpleZoom(
                    "シナリオ紹介シンプルズームカメラ", isLast);
            } else if (al::isEqualString(name, "ScenarioStartCameraRailMove")) {
                bool isLast = lastIndex <= i;
                poser = new ScenarioStartCameraPoserRailMove(
                    "シナリオ紹介レール移動カメラ", isLast);
            }
            al::CameraTicket* ticket =
                alCameraFunction::initCamera(poser, this, placementInfo, nullptr, 5);
            alCameraFunction::initPriorityDemo(ticket);
            poser->initScenarioCamera(placementInfo);
            mTickets[i] = ticket;
            if (isKeep)
                alCameraFunction::validateKeepPreSelfPoseNextCamera(mTickets[i]);
            if (lastIndex <= i)
                break;
            al::PlacementInfo nextPlacementInfo;
            al::getLinksInfo(&nextPlacementInfo, placementInfo, "NextKey");
            placementInfo = nextPlacementInfo;
        }
    }
    makeActorDead();
}

const sead::Matrix34f* ScenarioStartCamera::getBaseMtx() const {
    if (mBaseMtx)
        return mBaseMtx;
    return al::LiveActor::getBaseMtx();
}

void ScenarioStartCamera::appear() {
    al::LiveActor::appear();
    al::requestCaptureScreenCover(this, 4);
    for (s32 i = 0; i < mQuestInfos->size(); i++) {
        if (rs::isActiveQuest(mQuestInfos->at(i)))
            rs::playScenarioStartCamera(this, mQuestInfos->at(i));
        if (GameDataFunction::isCityWorldScenario2(this))
            rs::playScenarioStartCamera(this, 2);
    }
    al::tryOnStageSwitch(this, "SwitchAppearOn");
    al::tryOnStageSwitch(this, "SwitchAliveKeepOn");
    rs::requestValidateDemoSkip(this, this);
    if (mDemoCameraTicket) {
        al::startAnimCamera(this, mDemoCameraTicket, mDemoAnimName, 0);
        al::setNerve(this, &NrvScenarioStartCamera.PlayAnim);
    } else {
        mCurrentIndex = 0;
        al::startCamera(this, mTickets[0], 0);
        al::setNerve(this, &NrvScenarioStartCamera.Play);
    }
}

bool ScenarioStartCamera::isFirstDemo() const {
    return rs::isFirstDemoScenarioStartCamera(this);
}

bool ScenarioStartCamera::isEnableSkipDemo() const {
    return true;
}

void ScenarioStartCamera::initAfterPlacement() {
    sead::PtrArray<QuestInfo>* questInfos = mQuestInfos;
    if (questInfos) {
        s32 questNum = questInfos->size();
        bool isFirstQuest;
        if (questNum > 1)
            isFirstQuest = true;
        else if (questNum == 0)
            isFirstQuest = false;
        else
            isFirstQuest = GameDataFunction::getMainQuestMin(this) ==
                           questInfos->at(0)->getQuestNo();
        mIsFirstQuest = isFirstQuest;
    }
}

s32 ScenarioStartCamera::getMaxPlayStep() {
    return 480;
}

void ScenarioStartCamera::end() {
    if (mDemoCameraTicket) {
        al::endCamera(this, mDemoCameraTicket, 0, false);
    } else {
        al::CameraTicket* ticket = mTickets[mCurrentIndex];
        al::endCamera(this, ticket,
                      static_cast<ScenarioStartCameraPoser*>(ticket->getPoser())
                          ->getNextInterpoleStep(),
                      false);
    }
}

al::CameraPoser* ScenarioStartCamera::getPoser(s32 index) const {
    return mTickets[index]->getPoser();
}

void ScenarioStartCamera::kill() {
    al::LiveActor::kill();
    if (rs::isActiveDemo(this))
        rs::requestEndDemoScenarioCamera(this);
    al::tryOnSwitchDeadOn(this);
    al::tryOffStageSwitch(this, "SwitchAliveKeepOn");
}

void ScenarioStartCamera::exePlay() {
    s32 totalStep;
    s32 startStep;
    s32 endStep;
    s32 index;
    s32 endWaitStep;
    al::CameraTicket** tickets = mTickets;
    s32 ticketNum = mTicketNum;
    if (static_cast<ScenarioStartCameraPoser*>(tickets[0]->getPoser())->getPlayStep() < 1) {
        s32 playStep = 480 / ticketNum;
        index = al::calcNerveInterval(this, playStep, 0);
        s32 maxIndex = ticketNum - 1;
        index = index > maxIndex ? maxIndex : index;
        startStep = index * playStep;
        endWaitStep = 0;
        if (maxIndex <= index)
            endWaitStep = 60;
        endStep = (index + 1) * playStep + endWaitStep - 1;
        totalStep = 540;
    } else {
        index = 0;
        startStep = 0;
        endStep = 0;
        totalStep = 0;
        bool isFind = false;
        for (s32 i = 0; i < ticketNum; i++) {
            ScenarioStartCameraPoser* poser =
                static_cast<ScenarioStartCameraPoser*>(tickets[i]->getPoser());
            totalStep += poser->getPlayStep();
            if (!isFind && al::getNerveStep(this) < totalStep) {
                startStep = totalStep - poser->getPlayStep();
                index = i;
                endStep = totalStep;
                isFind = true;
            }
        }
        if (!isFind) {
            index = ticketNum - 1;
            startStep = totalStep - static_cast<ScenarioStartCameraPoser*>(
                                          tickets[ticketNum - 1]->getPoser())
                                          ->getPlayStep();
            endStep = totalStep;
        }
        endWaitStep = 0;
    }
    s32 waitStep =
        static_cast<ScenarioStartCameraPoser*>(mTickets[index]->getPoser())->getEndWaitStep();
    endStep = endStep - endWaitStep - waitStep;
    f32 rate = al::calcNerveEaseByTypeRate(
        this, startStep, endStep,
        static_cast<ScenarioStartCameraPoser*>(mTickets[index]->getPoser())
            ->getInterpoleType());
    static_cast<ScenarioStartCameraPoser*>(mTickets[index]->getPoser())->setInterpoleRate(rate);
    if (mCurrentIndex != index) {
        al::endCamera(this, mTickets[mCurrentIndex], -1, false);
        al::startCamera(this, mTickets[index],
                        static_cast<ScenarioStartCameraPoser*>(mTickets[mCurrentIndex]->getPoser())
                            ->getNextInterpoleStep());
        mCurrentIndex = index;
    }
    if (al::isGreaterEqualStep(this, totalStep)) {
        al::requestCaptureScreenCover(this, 4);
        al::setNerve(this, &End);
    }
}

void ScenarioStartCamera::exePlayAnim() {
    if (al::isEndAnimCamera(mDemoCameraTicket)) {
        al::requestCaptureScreenCover(this, 4);
        al::setNerve(this, &End);
    }
}

void ScenarioStartCamera::exeEnd() {
    end();
    kill();
}

void ScenarioStartCamera::skipDemo() {
    end();
    kill();
}

bool ScenarioStartCamera::isEnableStart() const {
    bool isEnable = false;
    if (al::isAlive(this))
        return isEnable;
    if (!mQuestInfos)
        return isEnable;
    for (s32 i = 0; i < mQuestInfos->size(); i++) {
        GameDataHolderAccessor accessor{this};
        isEnable |= GameDataFunction::isPlayScenarioCamera(accessor, mQuestInfos->at(i));
    }
    GameDataHolderAccessor accessor{this};
    if (GameDataFunction::isWorldCity(accessor) && rs::getActiveQuestNum(this) >= 1 &&
        (u32)(rs::getActiveQuestNo(this) - 2) <= 4) {
        GameDataHolderAccessor accessor2{this};
        isEnable = !GameDataFunction::isPlayAlreadyScenarioStartCamera(accessor2, 2);
    }
    return isEnable;
}
