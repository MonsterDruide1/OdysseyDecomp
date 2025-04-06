#include "Library/Clipping/ClippingActorInfoList.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ClippingActorInfoList::ClippingActorInfoList(s32 maxClippingInfo)
    : mMaxClippingInfo(maxClippingInfo) {
    mClippingActorInfo = new ClippingActorInfo*[maxClippingInfo];
    for (s32 i = 0; i < mMaxClippingInfo; i++)
        mClippingActorInfo[i] = nullptr;
}

void ClippingActorInfoList::add(ClippingActorInfo* clippingActorInfo) {
    mClippingActorInfo[mClippingInfoCount] = clippingActorInfo;
    mClippingInfoCount++;
}

ClippingActorInfo* ClippingActorInfoList::remove(LiveActor* liveActor) {
    s32 index;
    ClippingActorInfo* result = find(liveActor, &index);
    mClippingActorInfo[index] = mClippingActorInfo[mClippingInfoCount - 1];
    mClippingInfoCount--;
    return result;
}

ClippingActorInfo* ClippingActorInfoList::find(const LiveActor* liveActor, s32* index) const {
    for (s32 i = 0; i < mClippingInfoCount; i++) {
        if (mClippingActorInfo[i]->getLiveActor() == liveActor) {
            if (index)
                *index = i;
            return mClippingActorInfo[i];
        }
    }
    return mClippingActorInfo[0];
}

ClippingActorInfo* ClippingActorInfoList::tryFind(const LiveActor* liveActor) const {
    for (s32 i = mClippingInfoCount - 1; i >= 0; i--)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return mClippingActorInfo[i];
    return nullptr;
}

bool ClippingActorInfoList::isInList(const LiveActor* liveActor) const {
    for (s32 i = 0; i < mClippingInfoCount; i++)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return true;
    return false;
}

}  // Namespace al
