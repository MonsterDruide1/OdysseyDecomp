#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Base/StringUtil.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
class PlacementInfo;
struct ActorInitInfo;
}  // namespace al
class QuestInfo;
class QuestInfoHolder;

namespace rs {
QuestInfoHolder* getQuestInfoHolder(const al::IUseSceneObjHolder*);
QuestInfo* tryCreateAndRegisterQuestInfoToHolder(const al::LiveActor*, const al::ActorInitInfo&);
QuestInfo* createAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor*,
                                                           const al::PlacementInfo&, bool);
void invalidateQuest(const QuestInfo*);
sead::PtrArray<QuestInfo>*
tryCreateAndRegisterQuestInfoToHolderFromLinkedObj(const al::LiveActor*, const al::ActorInitInfo&);
void validateQuest(const QuestInfo*);
const QuestInfo* const* getActiveQuestList(const al::IUseSceneObjHolder*);
s32 getActiveQuestNum(const al::IUseSceneObjHolder*);
s32 getActiveQuestNumForMap(const al::IUseSceneObjHolder*);
bool isActiveQuestAllEqualNo(const al::IUseSceneObjHolder*);
al::StringTmp<128> getActiveQuestLabel(const al::IUseSceneObjHolder*);
al::StringTmp<128> getActiveQuestStageName(const al::IUseSceneObjHolder*);
bool isActiveQuest(const QuestInfo*);
bool isActiveQuest(const sead::PtrArray<QuestInfo>*);
s32 getActiveQuestNo(const al::IUseSceneObjHolder*);
}  // namespace rs
