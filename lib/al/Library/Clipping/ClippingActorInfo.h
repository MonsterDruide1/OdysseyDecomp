#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;

class ClippingRequestKeeper;
class ClippingJudge;
class LiveActor;
class PlacementId;
class ViewIdHolder;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor* actor);
    void setTypeToSphere(f32 radius, const sead::Vector3f* pos);
    void startClipped();
    void endClipped();
    void updateClipping(const ClippingJudge* clippingJudge);
    bool judgeClipping(const ClippingJudge* clippingJudge) const;
    void updateClipping(ClippingRequestKeeper* clippingRequestKeeper,
                        const ClippingJudge* clippingJudge);
    bool isGroupClipping() const;
    bool isGroupClippingInit() const;
    void setTypeToObb(const sead::BoundBox3f& boundBox, const sead::Matrix34f* matrix);
    void setGroupClippingId(const ActorInitInfo& clippingId);
    void setFarClipLevel20M();
    void updateFarClipLevel();
    void setFarClipLevelMax();
    bool isFarClipLevelMax() const;
    bool checkActiveViewGroupAny() const;
    void initViewGroup(const ViewIdHolder* viewIdHolder);
    void registerViewGroupFarClipFlag(const bool* flag);

    LiveActor* getLiveActor() const { return mLiveActor; }

    PlacementId* getPlacementId() const { return mPlacementId; }

    const ViewIdHolder* getViewIdHolder() const { return mViewIdHolder; }

    bool isGroupClippingEnable() const { return mIsEnableGroupClipping; }

    void setGroupClippingEnable(bool enable) { mIsEnableGroupClipping = enable; }

    // TODO: rename unnamed member variables
private:
    LiveActor* mLiveActor = nullptr;
    const sead::Vector3f* mTransPtr = nullptr;
    f32 mClippingRadius = 0.0f;
    const sead::Matrix34f* _18 = nullptr;
    sead::BoundBox3f _20;
    f32 _38 = 300.0f;
    PlacementId* mPlacementId = nullptr;
    s16 _48 = 1;
    s16 _4a = 1;
    s32 mFarClipFlagEntries = 0;
    s32 mFarClipFlagSize = 0;
    const bool** mFarClipFlags = nullptr;
    const ViewIdHolder* mViewIdHolder = nullptr;
    bool mIsEnableGroupClipping = true;
};
}  // namespace al
