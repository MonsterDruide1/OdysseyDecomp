#include "Library/Layout/LayoutPartsActorKeeper.h"
#include "Library/Layout/LayoutActor.h"

namespace al {
void LayoutPartsActorKeeper::resisterPartsActor(LayoutActor* actor) {}
void LayoutPartsActorKeeper::appear() {
    for (s32 i = 0; i < mNumActors; i++) {
        mPartsActors[i]->appear();
    }
}
void LayoutPartsActorKeeper::calcAnim(bool recursive) {
    for (s32 i = 0; i < mNumActors; i++) {
        mPartsActors[i]->calcAnim(recursive);
    }
}
LayoutPartsActorKeeper::LayoutPartsActorKeeper(s32 maxActors) : mMaxActors(maxActors) {
    mPartsActors = new LayoutActor*[maxActors];
}
}  // namespace al
