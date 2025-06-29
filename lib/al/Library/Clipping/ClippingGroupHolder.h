#pragma once

#include <basis/seadTypes.h>



namespace al {
class PlacementId;
class ClippingInfoGroup;
class ClippingJudge;

struct ClippingActorInfo;

class ClippingGroupHolder {
public:
    ClippingGroupHolder();
    void update(const ClippingJudge*);
    void createAndCount(ClippingActorInfo*);
    ClippingInfoGroup* tryFindGroup(const ClippingActorInfo*);
    void allocBuffer();
    void registerInfo(ClippingActorInfo*);
    void leave(ClippingActorInfo*);
    void reentry(ClippingActorInfo*);

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
    void setGroupId(const ClippingActorInfo*);
    bool isEqualGroupId(const PlacementId*) const;
    bool judgeClippingAll(const ClippingJudge*) const;
    void startClippedAll();
    void endClippedAll();

private:
    s32 mCount;
    s32 mSize;
    ClippingActorInfo** mClippingInfos;
    PlacementId* mGroupId;
    bool mIsClipped;
};

}  // namespace al
