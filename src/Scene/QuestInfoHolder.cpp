#include "Scene/QuestInfoHolder.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Scene/QuestInfo.h"
#include "System/GameDataFunction.h"

QuestInfoHolder::QuestInfoHolder(s32 capacity) : mCapacity(capacity) {
    mQuests = new QuestInfo[mCapacity];
    mActiveQuests = new QuestInfo*[mCapacity];
    mIsMainQuest = new bool[mCapacity];

    for (s32 i = 0; i < mCapacity; i++) {
        mActiveQuests[i] = nullptr;
        mIsMainQuest[i] = false;
    }
}

void QuestInfoHolder::clearAll() {
    for (s32 i = 0; i < mCapacity; i++) {
        mQuests[i].clear();
        mActiveQuests[i] = nullptr;
        mIsMainQuest[i] = false;
    }
    mSize = 0;
    mActiveQuestNum = 0;
}

void QuestInfoHolder::finalizeForScene() {
    mIsInitialized = false;
}

void QuestInfoHolder::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    mIsInitialized = true;
}

void QuestInfoHolder::initAfterPlacementQuestObj(s32 questNo) {
    mActiveMainQuestNo = -1;
    s32 foundQuestNo = -1;

    for (s32 i = 0; i <= mMaxMainQuestNo; i++) {
        bool hasMainQuest = false;
        for (s32 j = 0; j < mSize; j++) {
            s32 currentQuestNo = mQuests[j].getQuestNo();

            if (currentQuestNo == questNo)
                foundQuestNo = questNo;

            if (currentQuestNo == i && mQuests[j].isMainQuest()) {
                if (hasMainQuest | mQuests[j].getScenarioName().isEmpty())
                    mIsMainQuest[i] = true;
                else
                    hasMainQuest = true;
            }
        }
    }

    if (foundQuestNo > -1)
        updateActiveList(foundQuestNo);
    else
        clearMainQuest();

    if (GameDataFunction::isWorldCity(mQuests) && mActiveMainQuestNo == 8 &&
        GameDataFunction::getScenarioNoPlacement(mQuests) != 7 &&
        GameDataFunction::getScenarioNoPlacement(mQuests) != 11) {
        clearMainQuest();
    }
}

void QuestInfoHolder::updateActiveList(s32 questNo) {
    clearMainQuest();

    for (s32 i = 0; i < mSize; i++) {
        QuestInfo* quest = &mQuests[i];
        if (!quest->isInvalid() && quest->getQuestNo() == questNo) {
            mActiveQuests[mActiveQuestNum] = quest;
            mActiveQuestNum++;
            if (mQuests[i].isMainQuest())
                mActiveMainQuestNo = mQuests[i].getQuestNo();
            mActiveQuestNo = mQuests[i].getQuestNo();
        }
    }
}

void QuestInfoHolder::clearMainQuest() {
    for (s32 i = 0; i < mCapacity; i++)
        mActiveQuests[i] = nullptr;
    mActiveQuestNum = 0;
}

void QuestInfoHolder::initSceneObjHolder(al::SceneObjHolder* holder) {
    for (s32 i = 0; i < mCapacity; i++)
        mQuests[i].setSceneObjHolder(holder);
}

QuestInfo* QuestInfoHolder::registerQuestInfo(const QuestInfo* quest) {
    QuestInfo* newQuest = &mQuests[mSize];
    newQuest->copy(quest);
    mSize++;

    if (quest->isMainQuest())
        mMaxMainQuestNo = sead::Mathi::max(mMaxMainQuestNo, quest->getQuestNo());

    return newQuest;
}

void QuestInfoHolder::validateQuest(const QuestInfo* quest) {
    // BUG: No bounds checking. Will soflock if entry is not found.
    for (s32 i = 0; true; i++) {
        if (mQuests[i].isEqual(quest)) {
            mQuests[i].setInvalid(false);
            return;
        }
    }
}

void QuestInfoHolder::invalidateQuest(const QuestInfo* quest) {
    QuestInfo* entry = tryFindQuest(quest);

    if (entry->isInvalid())
        return;

    entry->end();

    if (mActiveQuestNo > entry->getQuestNo() || !mIsInitialized)
        return;

    updateActiveList(entry->getQuestNo());

    if (mActiveQuestNum == 0) {
        updateActiveList(entry->getQuestNo() + 1);
        GameDataFunction::setMainScenarioNo(quest, entry->getQuestNo() + 1);
    }
}

s32 QuestInfoHolder::getQuestNum(s32 questNo) const {
    s32 questNum = 0;
    for (s32 i = 0; i < mSize; i++)
        if (mQuests[i].getQuestNo() == questNo)
            questNum++;

    return questNum;
}

al::StringTmp<128> QuestInfoHolder::getActiveQuestLabel() const {
    if (mIsMainQuest[mActiveQuestNo])
        return al::StringTmp<128>("Quest_%02d", mActiveQuestNo);

    return mActiveQuests[0]->getScenarioName().cstr();
}

al::StringTmp<128>
QuestInfoHolder::getActiveQuestStageName(const al::IUseSceneObjHolder* holder) const {
    if (mIsMainQuest[mActiveQuestNo])
        return GameDataFunction::tryGetCurrentMainStageName(holder);

    return mActiveQuests[0]->getStageName().cstr();
}

bool QuestInfoHolder::isActiveQuest(const QuestInfo* quest) const {
    for (s32 i = 0; i < mActiveQuestNum; i++)
        if (mActiveQuests[i]->isEqual(quest))
            return true;

    return false;
}

QuestInfo* QuestInfoHolder::tryFindQuest(const QuestInfo* quest) const {
    for (s32 i = 0; i < mCapacity; i++)
        if (mQuests[i].isEqual(quest))
            return &mQuests[i];
    return nullptr;
}

QuestInfo* QuestInfoHolder::tryFindQuest(const al::PlacementInfo& placementInfo,
                                         al::SceneObjHolder* holder) const {
    QuestInfo quest;
    quest.init(placementInfo, holder);
    return tryFindQuest(&quest);
}

namespace rs {
QuestInfoHolder* getQuestInfoHolder(const al::IUseSceneObjHolder* holder) {
    return al::getSceneObj<QuestInfoHolder>(holder);
}

QuestInfo* tryCreateAndRegisterQuestInfoToHolder(const al::LiveActor* actor,
                                                 const al::ActorInitInfo& info) {
    s32 questNo = -1;
    al::tryGetArg(&questNo, info, "QuestNo");

    if (questNo == -1)
        return nullptr;

    QuestInfo* foundQuest = getQuestInfoHolder(actor)->tryFindQuest(
        al::getPlacementInfo(info), info.actorSceneInfo.sceneObjHolder);
    if (foundQuest)
        return foundQuest;

    QuestInfo quest;
    quest.init(info);
    return getQuestInfoHolder(actor)->registerQuestInfo(&quest);
}

QuestInfo* createAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor* actor,
                                                           const al::PlacementInfo& info,
                                                           bool isDummy) {
    s32 questNo = -1;
    al::tryGetArg(&questNo, info, "QuestNo");

    if (questNo == -1)
        return nullptr;

    QuestInfo* foundQuest =
        getQuestInfoHolder(actor)->tryFindQuest(info, actor->getSceneInfo()->sceneObjHolder);
    if (foundQuest)
        return foundQuest;

    QuestInfo quest;
    quest.init(info, actor->getSceneObjHolder());
    if (isDummy) {
        const char* stageName = nullptr;
        const char* objId = nullptr;
        al::getStringArg(&stageName, info, "StageName");
        al::getStringArg(&objId, info, "ObjId");
        quest.setStageName(stageName);
        quest.setLabel(objId);
    }

    QuestInfo* newQuest = getQuestInfoHolder(actor)->registerQuestInfo(&quest);
    if (isDummy) {
        const char* stageName = nullptr;
        const char* objId = nullptr;
        al::getStringArg(&stageName, info, "StageName");
        al::getStringArg(&objId, info, "ObjId");
        if (GameDataFunction::isGotShine(actor, stageName, objId))
            invalidateQuest(&quest);
    }

    return newQuest;
}

sead::PtrArray<QuestInfo>*
tryCreateAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor* actor,
                                                   const al::ActorInitInfo& info) {
    al::PlacementInfo placementInfo;
    s32 noDeleteShines = al::calcLinkChildNum(info, "NoDelete_Shine");
    s32 shineDummies = al::calcLinkChildNum(info, "ShineDummy");
    if (shineDummies + noDeleteShines == 0)
        return nullptr;

    sead::PtrArray<QuestInfo>* quests = new sead::PtrArray<QuestInfo>;
    quests->allocBuffer(shineDummies + noDeleteShines, nullptr);
    for (s32 i = 0; i < noDeleteShines; i++) {
        al::getLinksInfoByIndex(&placementInfo, info, "NoDelete_Shine", i);
        QuestInfo* quest =
            createAndRegisterQuestInfoToHolderFromLinkedObj(actor, placementInfo, false);
        if (quest)
            quests->pushBack(quest);
    }

    if (shineDummies < 1) {
        if (quests->isEmpty())
            return nullptr;

        return quests;
    }

    for (s32 i = 0; i < shineDummies; i++) {
        al::getLinksInfoByIndex(&placementInfo, info, "ShineDummy", i);
        QuestInfo* quest =
            createAndRegisterQuestInfoToHolderFromLinkedObj(actor, placementInfo, true);
        if (quest)
            quests->pushBack(quest);
    }

    if (quests->isEmpty())
        return nullptr;

    return quests;
}

void validateQuest(const QuestInfo* quest) {
    if (!quest)
        return;

    getQuestInfoHolder(quest)->validateQuest(quest);
}

void invalidateQuest(const QuestInfo* quest) {
    if (!quest)
        return;
    getQuestInfoHolder(quest)->invalidateQuest(quest);
}

const QuestInfo* const* getActiveQuestList(const al::IUseSceneObjHolder* holder) {
    return getQuestInfoHolder(holder)->getActiveQuestList();
}

s32 getActiveQuestNum(const al::IUseSceneObjHolder* holder) {
    return getQuestInfoHolder(holder)->getActiveQuestNum();
}

s32 getActiveQuestNumForMap(const al::IUseSceneObjHolder* holder) {
    s32 questNum = getActiveQuestNum(holder);
    bool isWorldSnow = GameDataFunction::isWorldSnow(holder);

    return (1 < questNum) & isWorldSnow ? 1 : questNum;
}

bool isActiveQuestAllEqualNo(const al::IUseSceneObjHolder* holder) {
    return getActiveQuestNum(holder) ==
           getQuestInfoHolder(holder)->getQuestNum(getActiveQuestNo(holder));
}

al::StringTmp<128> getActiveQuestLabel(const al::IUseSceneObjHolder* holder) {
    return getQuestInfoHolder(holder)->getActiveQuestLabel();
}

al::StringTmp<128> getActiveQuestStageName(const al::IUseSceneObjHolder* holder) {
    return getQuestInfoHolder(holder)->getActiveQuestStageName(holder);
}

bool isActiveQuest(const QuestInfo* quest) {
    if (!quest || !quest->QuestInfo::getSceneObjHolder())
        return false;

    return getQuestInfoHolder(quest)->isActiveQuest(quest);
}

bool isActiveQuest(const sead::PtrArray<QuestInfo>* questList) {
    if (!questList)
        return false;

    s32 size = questList->size();
    for (s32 i = 0; i < size; i++) {
        QuestInfo* quest = questList->at(i);
        if (!quest->QuestInfo::getSceneObjHolder())
            return false;
        if (getQuestInfoHolder(quest)->isActiveQuest(quest))
            return true;
    }

    return false;
}

s32 getActiveQuestNo(const al::IUseSceneObjHolder* holder) {
    return getQuestInfoHolder(holder)->getActiveQuestNo();
}
}  // namespace rs
