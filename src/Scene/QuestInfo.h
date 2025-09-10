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

    s32 getQuestNo() const { return mQuestNo; }

private:
    s32 mQuestNo;
    void* filler_18;
    bool mIsMainQuest;
    void* filler_28;
    sead::SafeString* mScenarioName;
    void* filler_c0[18];
    sead::SafeString* mStageName;
    void* filler_158[18];
    bool mIsSingle;
    void* filler_208[21];
};

static_assert(sizeof(QuestInfo) == 0x208);
