#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class Shine;

class YoshiFruitShineHolder : public al::LiveActor {
public:
    YoshiFruitShineHolder(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void updateHintPos(const sead::Vector3f& pos);
    Shine* appearShineFromFruit(const sead::Vector3f& pos);

    s32 getGotShineNum() const { return mGotShineNum; }

private:
    sead::PtrArray<Shine> mShines;
    s32 mGotShineNum = 0;
};

static_assert(sizeof(YoshiFruitShineHolder) == 0x120);
