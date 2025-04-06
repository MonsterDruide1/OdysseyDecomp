#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

enum ClippingRequestKeeper {};

namespace al {
class LiveActor;
class ActorInitInfo;
class ClippingJudge;
class ViewIdHolder;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor*);
    bool checkActiveViewGroupAny() const;
    void endClipping();
    void initViewGroup(const ViewIdHolder*);
    bool isFarClipLevelMax() const;
    bool isGroupClipping() const;
    bool isGroupClippingInit() const;
    bool JudgeClipping(const ClippingJudge*) const;
    void registerViewGroupFarClipFlag(const bool*);
    void setFarClipLevel20M();
    void setFarClipLevelMax();
    void setGroupClippingId(const ActorInitInfo&);
    void setTypeToObb(const sead::BoundBox3f&, const sead::Matrix34f*);
    void setTypeToSphere(f32, const sead::Vector3f*);
    void startClipped();
    void updateClipping(const ClippingJudge*);
    void updateClipping(ClippingRequestKeeper*, const ClippingJudge*);
    void updateFarClipLevel();

    const LiveActor* getLiveActor() const { return mLiveActor; }

private:
    LiveActor* mLiveActor;
    // Remaining Member's missing
};

}  // namespace al