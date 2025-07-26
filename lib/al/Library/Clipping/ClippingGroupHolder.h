#pragma once

#include <basis/seadTypes.h>

namespace al {
class PlacementId;
class ClippingInfoGroup;
class ClippingJudge;

class ClippingActorInfo;

class ClippingGroupHolder {
public:
    ClippingGroupHolder();
    void update(const ClippingJudge* clippingJudge);
    void createAndCount(ClippingActorInfo* clippingActorInfo);
    ClippingInfoGroup* tryFindGroup(const ClippingActorInfo* clippingActorInfo);
    void allocBuffer();
    void registerInfo(ClippingActorInfo* clippingActorInfo);
    void leave(ClippingActorInfo* clippingActorInfo);
    void reentry(ClippingActorInfo* clippingActorInfo);

private:
    void* filler[0x2];
};

class ClippingInfoGroup {
public:
    ClippingInfoGroup();
    void registerInfo(ClippingActorInfo* clippingActorInfo);
    void removeInfo(ClippingActorInfo* clippingActorInfo);
    void addCount();
    void allocBuffer();
    void setGroupId(const ClippingActorInfo* clippingActorInfo);
    bool isEqualGroupId(const PlacementId* placementId) const;
    bool judgeClippingAll(const ClippingJudge* clippingJudge) const;
    void startClippedAll();
    void endClippedAll();

private:
    s32 mCount = 0;
    s32 mSize = 0;
    ClippingActorInfo** mClippingInfos = nullptr;
    PlacementId* mGroupId;
    bool mIsClipped = false;
};

}  // namespace al
