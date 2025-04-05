#include "Library/Clipping/ClippingActorInfoList.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
void ClippingActorInfoList::add(ClippingActorInfo* clippingActorInfo) {
    mClippingActorInfo[mClippingInfoCount] = clippingActorInfo;
    mClippingInfoCount++;
}

ClippingActorInfoList::ClippingActorInfoList(s32 maxClippingInfo)
    : mMaxClippingInfo(maxClippingInfo), mClippingInfoCount(0) {
    mClippingActorInfo = new ClippingActorInfo*[maxClippingInfo];
    for (s32 i = 0; i < mMaxClippingInfo; i++)
        mClippingActorInfo[i] = nullptr;
}

ClippingActorInfo* ClippingActorInfoList::find(const LiveActor* liveActor, s32* index) const {
    for (s32 i = 0; i < mClippingInfoCount; i++) {
        if (mClippingActorInfo[i]->getLiveActor() != liveActor)
            continue;
        if (index)
            *index = i;
        return mClippingActorInfo[i];
    }
    return mClippingActorInfo[0];
}

bool ClippingActorInfoList::isInList(const LiveActor* liveActor) const {
    if (mClippingInfoCount < 1)
        return false;
    for (s32 i{}; i < mClippingInfoCount; i++)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return true;
    return false;
}

ClippingActorInfo* ClippingActorInfoList::remove(LiveActor* liveActor) {
    s32 index;
    ClippingActorInfo* result = find(liveActor, &index);
    mClippingActorInfo[index] = mClippingActorInfo[mClippingInfoCount - 1];
    mClippingInfoCount--;
    return result;
}

ClippingActorInfo* ClippingActorInfoList::tryFind(const LiveActor* liveActor) const {
    for (s32 i = mClippingInfoCount - 1; i >= 0; --i)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return mClippingActorInfo[i];
    return nullptr;
}

}  // Namespace al
