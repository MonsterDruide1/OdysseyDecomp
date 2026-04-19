#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Base/StringUtil.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
class PlacementInfo;
}  // namespace al
class QuestInfo;

class QuestInfoHolder : public al::ISceneObj, public al::HioNode {
public:
    static constexpr s32 sSceneObjId = SceneObjID_QuestInfoHolder;

    QuestInfoHolder(s32 capacity);

    void clearAll();
    void finalizeForScene();
    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void initAfterPlacementQuestObj(s32 questNo);
    void updateActiveList(s32 questNo);
    void clearMainQuest();
    void initSceneObjHolder(al::SceneObjHolder* holder);
    QuestInfo* registerQuestInfo(const QuestInfo* quest);
    void validateQuest(const QuestInfo* quest);
    void invalidateQuest(const QuestInfo* quest);
    s32 getQuestNum(s32 questNo) const;
    al::StringTmp<128> getActiveQuestLabel() const;
    al::StringTmp<128> getActiveQuestStageName(const al::IUseSceneObjHolder* holder) const;
    bool isActiveQuest(const QuestInfo* quest) const;
    QuestInfo* tryFindQuest(const QuestInfo* quest) const;
    QuestInfo* tryFindQuest(const al::PlacementInfo& placementInfo,
                            al::SceneObjHolder* holder) const;

    const char* getSceneObjName() const override { return "クエスト情報保持者"; }

    s32 getActiveQuestNo() const { return mActiveQuestNo; }

    s32 getActiveQuestNum() const { return mActiveQuestNum; }

    QuestInfo** getActiveQuestList() const { return mActiveQuests; }

private:
    s32 mCapacity;
    QuestInfo* mQuests = nullptr;
    s32 mSize = 0;
    s32 mActiveMainQuestNo = -1;
    s32 mMaxMainQuestNo = -1;
    s32 mActiveQuestNo = -1;
    QuestInfo** mActiveQuests = nullptr;
    s32 mActiveQuestNum = 0;
    bool mIsInitialized = false;
    bool* mIsMainQuest = nullptr;
};

static_assert(sizeof(QuestInfoHolder) == 0x40);

namespace rs {
QuestInfoHolder* getQuestInfoHolder(const al::IUseSceneObjHolder* holder);
QuestInfo* tryCreateAndRegisterQuestInfoToHolder(const al::LiveActor* actor,
                                                 const al::ActorInitInfo& info);
QuestInfo* createAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor* actor,
                                                           const al::PlacementInfo& placementInfo,
                                                           bool is_dummy);
sead::PtrArray<QuestInfo>*
tryCreateAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor* actor,
                                                   const al::ActorInitInfo& info);
void validateQuest(const QuestInfo* quest);
void invalidateQuest(const QuestInfo* quest);
const QuestInfo* const* getActiveQuestList(const al::IUseSceneObjHolder* holder);
s32 getActiveQuestNum(const al::IUseSceneObjHolder* holder);
s32 getActiveQuestNumForMap(const al::IUseSceneObjHolder* holder);
bool isActiveQuestAllEqualNo(const al::IUseSceneObjHolder* holder);
al::StringTmp<128> getActiveQuestLabel(const al::IUseSceneObjHolder* holder);
al::StringTmp<128> getActiveQuestStageName(const al::IUseSceneObjHolder* holder);
bool isActiveQuest(const QuestInfo* quest);
bool isActiveQuest(const sead::PtrArray<QuestInfo>* list);
s32 getActiveQuestNo(const al::IUseSceneObjHolder* holder);
}  // namespace rs
