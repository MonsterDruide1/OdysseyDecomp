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

struct ClippingActorInfo {
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

    LiveActor* liveActor;
    sead::Vector3f _8;
    f32 _10;
    sead::Matrix34f* _18;
    sead::BoundBox3f _20;
    f32 _38;
    PlacementId* placementId;
    s16 _48;
    s16 _4a;
    s32 _4c;
    s32 _50;
    void* _58;
    ViewIdHolder* viewIdHolder;
    bool isEnableGroupClipping;
};
}  // namespace al
