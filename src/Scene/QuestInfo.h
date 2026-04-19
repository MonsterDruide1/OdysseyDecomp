#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
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
    void init(const al::ActorInitInfo& initInfo);
    void init(const al::PlacementInfo& placementInfo, const al::ActorInitInfo& initInfo);
    void init(const al::PlacementInfo& placementInfo, al::SceneObjHolder* holder);
    void setStageName(const char* stageName);
    void setLabel(const char* label);
    void copy(const QuestInfo* quest);
    void end();
    bool isEqual(const QuestInfo* quest) const;

    s32 getQuestNo() const { return mQuestNo; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    bool isMainQuest() const { return mIsMainQuest; }

    bool isInvalid() const { return mIsInvalid; }

    void setInvalid(bool isInvalid) { mIsInvalid = isInvalid; }

    void setSceneObjHolder(al::SceneObjHolder* holder) { mSceneObjHolder = holder; }

    al::SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

    const sead::FixedSafeString<128>& getScenarioName() const { return mScenarioName; }

    const sead::FixedSafeString<128>& getStageName() const { return mStageName; }

    bool isSingle() const { return mIsSingle; }

    const sead::FixedSafeString<128>& getPlacementId() const { return mPlacementId; }

    const sead::FixedSafeString<128>& getPlacementStageName() const { return mPlacementStageName; }

private:
    s32 mQuestNo = -1;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    bool mIsMainQuest = false;
    bool mIsInvalid = false;
    al::SceneObjHolder* mSceneObjHolder = nullptr;
    sead::FixedSafeString<128> mScenarioName;
    sead::FixedSafeString<128> mStageName;
    bool mIsSingle = false;
    sead::FixedSafeString<128> mPlacementId;
    sead::FixedSafeString<128> mPlacementStageName;
};

static_assert(sizeof(QuestInfo) == 0x290);
