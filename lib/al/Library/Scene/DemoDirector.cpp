#include "Library/Scene/DemoDirector.h"

#include "Library/Effect/EffectSystem.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {

AddDemoInfo::AddDemoInfo(s32 size) : mActorListSize{size} {
    mActorList = new LiveActor*[mActorListSize];
    for (s32 i = 0; i < mActorListSize; i++)
        mActorList[i] = nullptr;
}

void AddDemoInfo::init(const PlacementId& placementId) {
    mPlacementId = new PlacementId();
    *mPlacementId = placementId;
}

void AddDemoInfo::addDemoActor(LiveActor* actor) {
    mActorList[mActorListCount] = actor;
    mActorListCount++;
}

const char* AddDemoInfo::getDemoName() const {
    return mName ?: mPlacementId->getId();
}

void AddDemoInfo::reset() {
    mName = nullptr;
    for (s32 i = 0; i < mActorListCount; i++)
        mActorList[i] = nullptr;
    mActorListCount = 0;
}

LiveActor* AddDemoInfo::getDemoActor(s32 index) const {
    return mActorList[index];
}

}  // namespace al
