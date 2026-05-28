#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Shine;

class YoshiFruitShineHolder : public al::LiveActor {
public:
    YoshiFruitShineHolder(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void updateHintPos(const sead::Vector3f& pos);
    al::LiveActor* appearShineFromFruit(const sead::Vector3f& pos);

private:
    sead::PtrArray<Shine> mShines;
    s32 mAppearedCount = 0;
};

static_assert(sizeof(YoshiFruitShineHolder) == 0x120);

namespace rs {
void registerFruitShineHolder(YoshiFruitShineHolder*);
}  // namespace rs
