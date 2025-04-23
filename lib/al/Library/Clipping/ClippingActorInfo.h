#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

enum ClippingRequestKeeper {};

namespace al {
class LiveActor;
struct ActorInitInfo;

class ClippingJudge;
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

    ViewIdHolder* getViewIdHolder() const { return mViewIdHolder; }

private:
    LiveActor* mLiveActor;
    const void* filler[0xb];
    ViewIdHolder* mViewIdHolder;

    // Remaining Member's missing
};

}  // namespace al
