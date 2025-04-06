#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

enum ClippingRequestKeeper {};

namespace al {
class LiveActor;
<<<<<<< HEAD
<<<<<<< HEAD
struct ActorInitInfo;
=======
class ActorInitInfo;
>>>>>>> 9128333a (Stashing as im dealing with multiple pr's rn)
=======
struct ActorInitInfo;
>>>>>>> 8ff7bde3 (More uploading to switch branches)
class ClippingJudge;
class ViewIdHolder;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor*);
<<<<<<< HEAD
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
=======
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
<<<<<<< HEAD
>>>>>>> 9128333a (Stashing as im dealing with multiple pr's rn)
=======
    void setIsClipping(bool value) { mIsClippingEnable = value; }
>>>>>>> 8ff7bde3 (More uploading to switch branches)

private:
    LiveActor* mLiveActor;
    void* filler[0xc]; // offset isClippingEnable to the correct place
    bool mIsClippingEnable;
    // Remaining Member's missing
};

<<<<<<< HEAD
}  // namespace al
=======
}  // namespace al
>>>>>>> 9128333a (Stashing as im dealing with multiple pr's rn)
