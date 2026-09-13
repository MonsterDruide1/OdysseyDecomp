#include "Item/YoshiFruitShineHolder.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Item/Shine.h"
#include "Util/ItemUtil.h"

YoshiFruitShineHolder::YoshiFruitShineHolder(const char* name) : al::LiveActor(name) {}

void YoshiFruitShineHolder::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);

    s32 shineNum = al::calcLinkChildNum(info, "ShineFromFruit");
    mAppearedCount = 0;
    mShines.allocBuffer(shineNum, nullptr);

    for (s32 i = 0; i < shineNum; i++) {
        Shine* shine = rs::initLinkShine(info, "ShineFromFruit", i);
        mShines.pushBack(shine);
        if (shine->isGot())
            mAppearedCount++;
    }

    rs::registerFruitShineHolder(this);
    makeActorAlive();
}

void YoshiFruitShineHolder::updateHintPos(const sead::Vector3f& pos) {
    for (Shine& shine : mShines)
        rs::updateHintTrans(&shine, pos);
}

Shine* YoshiFruitShineHolder::appearShineFromFruit(const sead::Vector3f& pos) {
    Shine* shine = mShines[mAppearedCount];

    al::resetPosition(shine, pos);
    shine->appearStatic();
    mAppearedCount++;
    return shine;
}
