#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class IUseSceneObjHolder;
class LiveActor;
class PlacementInfo;
struct ActorInitInfo;
}  // namespace al
class QuestInfo;

namespace rs {
void getQuestInfoHolder(const al::IUseSceneObjHolder*);
void tryCreateAndRegisterQuestInfoToHolder(const al::LiveActor*, const al::ActorInitInfo&);
void createAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor*, const al::PlacementInfo&,
                                                     bool);
void invalidateQuest(const QuestInfo*);
void tryCreateAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor*,
                                                        const al::ActorInitInfo&);
void validateQuest(const QuestInfo*);
const QuestInfo* const* getActiveQuestList(const al::IUseSceneObjHolder*);
s32 getActiveQuestNum(const al::IUseSceneObjHolder*);
s32 getActiveQuestNumForMap(const al::IUseSceneObjHolder*);
bool isActiveQuestAllEqualNo(const al::IUseSceneObjHolder*);
const char* getActiveQuestLabel(const al::IUseSceneObjHolder*);
const char* getActiveQuestStageName(const al::IUseSceneObjHolder*);
bool isActiveQuest(const QuestInfo*);
bool isActiveQuest(const sead::PtrArray<QuestInfo>*);
s32 getActiveQuestNo(const al::IUseSceneObjHolder*);
}  // namespace rs
