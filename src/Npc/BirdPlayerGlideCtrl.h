#pragma once

#include <container/seadStrTreeMap.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

class Bird;

class BirdPlayerGlideCtrl : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_BirdPlayerGlideCtrl;

    BirdPlayerGlideCtrl(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void validateGlideOnNose();
    void invalidateGlideOnNose();
    void validateGlideOnSitDownHead();
    void invalidateGlideOnSitDownHead();
    void addDemoActorAndFlyAway();

    void exeInvalid();
    void exeValidOnNose();
    void exeValidOnSitDownHead();
    void exeWaitFlyAway();

    const char* getSceneObjName() const override { return getName(); }

    bool isValidOnSitDownHead() const { return mIsValidOnSitDownHead; }

private:
    sead::StrTreeMap<32, sead::Vector3f> mCapOffsetInfo;
    sead::StrTreeMap<32, sead::Vector3f> mCostumeOffsetScaleInfo;
    Bird* mBird = nullptr;
    Bird* mCommonBird = nullptr;
    Bird* mUfoBird = nullptr;
    sead::Matrix34f mDestinationMtx = sead::Matrix34f::ident;
    sead::Vector3f mCapOffset;
    sead::Vector3f mCostumeOffsetScale = {1, 1, 1};
    s32 mGlideDownTime = 0;
    bool mIsValidOnNose = false;
    bool mIsValidOnSitDownHead = false;
};

static_assert(sizeof(BirdPlayerGlideCtrl) == 0x1b8);

namespace rs {
void validateGlideBirdOnPlayerNose(const al::LiveActor* player);
void invalidateGlideBirdOnPlayerNose(const al::LiveActor* player);
void validateGlideBirdOnSitDownPlayerHead(const al::LiveActor* player);
void invalidateGlideBirdOnSitDownPlayerHead(const al::LiveActor* player);
bool isPlayerSitDownChair(const Bird* bird);
}  // namespace rs
