#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ClippingActorInfo;

class ClippingActorInfoList {
public:
    ClippingActorInfoList(s32 param);
    void add(ClippingActorInfo* clippingActorInfo);
    ClippingActorInfo* find(const LiveActor* liveActor, s32* index) const;
    bool isInList(const LiveActor* liveActor) const;
    ClippingActorInfo* remove(LiveActor*);
    ClippingActorInfo* tryFind(const LiveActor* liveActor) const;

private:
    s32 mMaxClippingInfo;
    s32 mClippingInfoCount;
    ClippingActorInfo** mClippingActorInfo;
};
}  // namespace al
