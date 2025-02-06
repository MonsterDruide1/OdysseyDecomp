#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyMoveMapParts;

template <class T>
class DeriveActorGroup;

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
