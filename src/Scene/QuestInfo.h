#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
struct ActorInitInfo;
class PlacementInfo;
class SceneObjHolder;
}  // namespace al

class QuestInfo : public al::IUseSceneObjHolder {
public:
    QuestInfo();
    void clear();
    void init(const al::ActorInitInfo&);
    void init(const al::PlacementInfo&, const al::ActorInitInfo&);
    void init(const al::PlacementInfo&, al::SceneObjHolder*);
    void setStageName(const char*);
    void setLabel(const char*);
    void copy(const QuestInfo*);
    void end();
    bool isEqual(const QuestInfo*) const;

    al::SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

    s32 getQuestNo() const { return mQuestNo; }

private:
    s32 mQuestNo;
    void* filler_10;
    bool mIsMainQuest;
    al::SceneObjHolder* mSceneObjHolder;
    sead::SafeString* mScenarioName;
    void* filler_30[0x12];
    sead::SafeString* mStageName;
    void* filler_c8[0x12];
    bool mIsSingle;
    void* filler_160[0x26];
};

static_assert(sizeof(QuestInfo) == 0x290);
