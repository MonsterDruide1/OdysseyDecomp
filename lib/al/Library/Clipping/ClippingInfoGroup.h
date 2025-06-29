#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ClippingActorInfo;
class ClippingJudge;
class PlacementId;

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
