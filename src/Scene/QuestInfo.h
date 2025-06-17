#pragma once

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
class PlacementInfo;
struct ActorInitInfo;
}  // namespace al

class QuestInfo : public al::IUseSceneObjHolder {
public:
    QuestInfo();
    void clear();
    void init(const al::ActorInitInfo& actor_info);
    void init(const al::PlacementInfo& placement_info, const al::ActorInitInfo& actor_info);
    void init(const al::PlacementInfo& placement_info, al::SceneObjHolder* scene_obj_holder);
    void setStageName(const char* stage_name);
    void setLabel(const char* obj_id);
    void copy(const QuestInfo* other);
    void end();
    bool isEqual(const QuestInfo* other) const;

    s32 getQuestNo() const { return mQuestNo; }

    bool isMainQuest() const { return mIsMainQuest; }

    al::SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

private:
    s32 mQuestNo = -1;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    bool mIsMainQuest = false;
    bool _0x19 = false;
    al::SceneObjHolder* mSceneObjHolder = nullptr;
    sead::FixedSafeString<128> mLabel;
    sead::FixedSafeString<128> mStageName;
    bool mIsSingle = false;
    sead::FixedSafeString<128> mObjId;
    sead::FixedSafeString<128> mStageName2;
};
