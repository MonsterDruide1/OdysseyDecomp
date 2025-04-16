#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

enum ClippingRequestKeeper {};

namespace al {
class ClippingJudge;
class LiveActor;
class ViewIdHolder;
struct ActorInitInfo;

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

private:
    LiveActor* mLiveActor;
    // Remaining Member's missing
};

}  // namespace al
