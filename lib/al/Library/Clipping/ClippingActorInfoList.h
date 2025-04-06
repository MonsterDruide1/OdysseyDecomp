#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ClippingActorInfo;

class ClippingActorInfoList {
public:
    void add(ClippingActorInfo* clippingActorInfo);
    ClippingActorInfoList(s32 param);
    ClippingActorInfo* find(const LiveActor* liveActor, s32* index) const;
    bool isInList(const LiveActor* liveActor) const;
    ClippingActorInfo* remove(LiveActor* liveActor);
    ClippingActorInfo* tryFind(const LiveActor* liveActor) const;

private:
    s32 mMaxClippingInfo = 0;
    s32 mClippingInfoCount = 0;
    ClippingActorInfo** mClippingActorInfo;
};
}  // namespace al
