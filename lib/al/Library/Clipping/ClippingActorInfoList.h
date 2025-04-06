#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ClippingActorInfo;

class ClippingActorInfoList {
public:
    ClippingActorInfoList(s32 param);
    void add(ClippingActorInfo* clippingActorInfo);
    ClippingActorInfo* remove(LiveActor* liveActor);
    ClippingActorInfo* find(const LiveActor* liveActor, s32* index) const;
    ClippingActorInfo* tryFind(const LiveActor* liveActor) const;
    bool isInList(const LiveActor* liveActor) const;
private:
    s32 mMaxClippingInfo = 0;
    s32 mClippingInfoCount = 0;
    ClippingActorInfo** mClippingActorInfo;
};
}  // namespace al
