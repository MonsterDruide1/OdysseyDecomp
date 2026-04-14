#include "MapObj/QuestObj.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Scene/QuestInfo.h"
#include "Scene/QuestInfoHolder.h"
#include "System/GameDataUtil.h"
#include "System/SaveObjInfo.h"

QuestObj::QuestObj(const char* name) : al::LiveActor(name) {}

void QuestObj::init(const al::ActorInitInfo& info) {
    using QuestObjFunctor = al::FunctorV0M<QuestObj*, void (QuestObj::*)()>;

    al::initActor(this, info);
    mQuestInfo = rs::tryCreateAndRegisterQuestInfoToHolder(this, info);
    if (!mQuestInfo) {
        makeActorDead();
        return;
    }

    bool isIgnoreSave = false;
    al::tryGetArg(&isIgnoreSave, info, "IsIgnoreSave");
    if (!isIgnoreSave)
        mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);

    s32 isQuestInvalidatedBySave = 0;
    if (mSaveObjInfo && mQuestInfo->isMainQuest() && rs::isOnSaveObjInfo(mSaveObjInfo)) {
        rs::invalidateQuest(mQuestInfo);
        isQuestInvalidatedBySave = 1;
    }

    if (al::listenStageSwitchOnStart(this, QuestObjFunctor(this, &QuestObj::switchStart))) {
        al::PlacementInfo placementInfo;
        al::getLinksInfo(&placementInfo, info, "SwitchStart");
        mSwitchStartSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info, placementInfo);
        s32 isStartSaveOn = rs::isOnSaveObjInfo(mSwitchStartSaveObjInfo);
        if (isQuestInvalidatedBySave) {
        } else if (isStartSaveOn) {
            mIsQuestStarted = true;
            rs::validateQuest(mQuestInfo);
            if (mSwitchStartSaveObjInfo)
                rs::onSaveObjInfo(mSwitchStartSaveObjInfo);
        }
    } else {
        mIsQuestStarted = true;
        rs::validateQuest(mQuestInfo);
        if (mSwitchStartSaveObjInfo)
            rs::onSaveObjInfo(mSwitchStartSaveObjInfo);
    }

    if (al::listenStageSwitchOnStop(this, QuestObjFunctor(this, &QuestObj::switchStop)))
        mIsQuestStopped = true;

    initHitSensor(1);
    al::addHitSensorMapObj(this, info, "Body", 0.0f, 64, {0.0f, 0.0f, 0.0f});
    makeActorDead();
}

void QuestObj::switchStart() {
    mIsQuestStarted = true;
    rs::validateQuest(mQuestInfo);
    if (mSwitchStartSaveObjInfo)
        return rs::onSaveObjInfo(mSwitchStartSaveObjInfo);
}

void QuestObj::start() {
    mIsQuestStarted = true;
    rs::validateQuest(mQuestInfo);
    if (mSwitchStartSaveObjInfo)
        return rs::onSaveObjInfo(mSwitchStartSaveObjInfo);
}

void QuestObj::switchStop() {
    rs::invalidateQuest(mQuestInfo);
    if (mSaveObjInfo)
        return rs::onSaveObjInfo(mSaveObjInfo);
}

void QuestObj::initAfterPlacement() {}

bool QuestObj::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgSwitchKillOnInit(message)) {
        mIsQuestStopped = true;
        rs::invalidateQuest(mQuestInfo);
        if (mSaveObjInfo)
            rs::onSaveObjInfo(mSaveObjInfo);
        return true;
    }

    if (al::isMsgSwitchKillOffInit(message)) {
        mIsQuestStopped = true;
        mIsQuestStarted = true;
        rs::validateQuest(mQuestInfo);
        if (mSwitchStartSaveObjInfo)
            rs::onSaveObjInfo(mSwitchStartSaveObjInfo);
        return true;
    }

    if (!al::isMsgSwitchKillOn(message))
        return false;

    rs::invalidateQuest(mQuestInfo);
    if (mSaveObjInfo)
        rs::onSaveObjInfo(mSaveObjInfo);
    return true;
}
