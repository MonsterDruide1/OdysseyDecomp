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
    ClippingActorInfo(LiveActor*);
    void setTypeToSphere(f32, const sead::Vector3f*);
    void startClipped();
    void endClipped();
    void updateClipping(const ClippingJudge*);
    bool judgeClipping(const ClippingJudge*) const;
    void updateClipping(ClippingRequestKeeper*, const ClippingJudge*);
    bool isGroupClipping() const;
    bool isGroupClippingInit() const;
    void setTypeToObb(const sead::BoundBox3f&, const sead::Matrix34f*);
    void setGroupClippingId(const ActorInitInfo&);
    void setFarClipLevel20M();
    void updateFarClipLevel();
    void setFarClipLevelMax();
    bool isFarClipLevelMax() const;
    bool checkActiveViewGroupAny() const;
    void initViewGroup(const ViewIdHolder*);
    void registerViewGroupFarClipFlag(const bool*);

    LiveActor* getLiveActor() const { return mLiveActor; }

    PlacementId* getPlacementId() const { return mPlacementId; }

    ViewIdHolder* getViewIdHolder() const { return mViewIdHolder; }

    bool isGroupClippingEnable() const { return mIsEnableGroupClipping; }

    void setGroupClippingEnable(const bool i) { mIsEnableGroupClipping = i; }

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
    ViewIdHolder* mViewIdHolder = nullptr;
    bool mIsEnableGroupClipping = true;
};
}  // namespace al
