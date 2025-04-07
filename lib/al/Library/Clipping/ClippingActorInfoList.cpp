#include "Library/Clipping/ClippingActorInfoList.h"

#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ClippingActorInfoList::ClippingActorInfoList(s32 numClippingActorInfo)
    : mClippingActorInfoSize(numClippingActorInfo) {
    mClippingActorInfo = new ClippingActorInfo*[numClippingActorInfo];
    for (s32 i = 0; i < mClippingActorInfoSize; i++)
        mClippingActorInfo[i] = nullptr;
}

void ClippingActorInfoList::add(ClippingActorInfo* clippingActorInfo) {
    mClippingActorInfo[mClippingActorInfoCount] = clippingActorInfo;
    mClippingActorInfoCount++;
}

ClippingActorInfo* ClippingActorInfoList::remove(LiveActor* liveActor) {
    s32 index;
    ClippingActorInfo* result = find(liveActor, &index);
    mClippingActorInfo[index] = mClippingActorInfo[mClippingActorInfoCount - 1];
    mClippingActorInfoCount--;
    return result;
}

ClippingActorInfo* ClippingActorInfoList::find(const LiveActor* liveActor, s32* index) const {
    for (s32 i = 0; i < mClippingActorInfoCount; i++) {
        if (mClippingActorInfo[i]->getLiveActor() == liveActor) {
            if (index)
                *index = i;
            return mClippingActorInfo[i];
        }
    }
    return mClippingActorInfo[0];
}

ClippingActorInfo* ClippingActorInfoList::tryFind(const LiveActor* liveActor) const {
    for (s32 i = mClippingActorInfoCount - 1; i >= 0; i--)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return mClippingActorInfo[i];
    return nullptr;
}

bool ClippingActorInfoList::isInList(const LiveActor* liveActor) const {
    for (s32 i = 0; i < mClippingActorInfoCount; i++)
        if (mClippingActorInfo[i]->getLiveActor() == liveActor)
            return true;
    return false;
}

}  // Namespace al
