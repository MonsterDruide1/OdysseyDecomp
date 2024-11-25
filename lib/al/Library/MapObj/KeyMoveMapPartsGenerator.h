#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"

namespace al {
class KeyMoveMapParts;

class KeyMoveMapPartsGenerator : public LiveActor {
public:
    KeyMoveMapPartsGenerator(const char* name);

    void init(const ActorInitInfo& info) override;

    void exeDelay();
    void exeGenerate();

private:
    DeriveActorGroup<KeyMoveMapParts>* mKeyMoveMapPartsGroup = nullptr;
    sead::Vector3f mClippingTrans = sead::Vector3f::zero;
    s32 mDelayTime = 0;
    s32 mGenerateInterval = 60;
};
}  // namespace al
