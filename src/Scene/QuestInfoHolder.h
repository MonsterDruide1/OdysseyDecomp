#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class PlacementInfo;
class SceneObjHolder;
}  // namespace al

class QuestInfo;

class QuestInfoHolder {
public:
    QuestInfoHolder(s32);

    void clearAll();
    void finalizeForScene();
    void initAfterPlacementSceneObj(const al::ActorInitInfo&);
    void initAfterPlacementQuestObj(s32);
    void updateActiveList(s32);
    void clearMainQuest();
    void initSceneObjHolder(al::SceneObjHolder*);
    void registerQuestInfo(const QuestInfo*);
    void validateQuest(const QuestInfo*);
    void tryFindQuest(const QuestInfo*) const;
    void invalidateQuest(const QuestInfo*);
    void getQuestNum(s32) const;
    void getActiveQuestLabel() const;
    void getActiveQuestStageName(const al::IUseSceneObjHolder*) const;
    bool isActiveQuest(const QuestInfo*) const;
    void tryFindQuest(const al::PlacementInfo&, al::SceneObjHolder*) const;
    void getSceneObjName() const;

private:
    char filler[0x40];
};

static_assert(sizeof(QuestInfoHolder) == 0x40);

namespace rs {
s32 getActiveQuestNum(const al::IUseSceneObjHolder*);
s32 getActiveQuestNo(const al::IUseSceneObjHolder*);
const char* getActiveQuestLabel(const al::IUseSceneObjHolder*);
}  // namespace rs
