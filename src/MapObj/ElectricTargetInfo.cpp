#include "MapObj/ElectricTargetInfo.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorPoseUtil.h"

f32 TargetInfo::dist() const {
    sead::Vector3f targetPos = al::getTrans(target);
    const sead::Vector3f& actorPos = al::getTrans(actor);
    return (targetPos - actorPos).length();
}

TargetInfoList::TargetInfoList() {
    mInfos = new sead::PtrArray<TargetInfo>();
    mPool = new sead::PtrArray<TargetInfo>();
    mInfos->allocBuffer(8, nullptr);
    mPool->allocBuffer(8, nullptr);
    for (s32 i = 0; i < 8; i++)
        mPool->pushBack(new TargetInfo());
}

void TargetInfoList::clear() {
    while (mInfos->size() != 0)
        mPool->pushBack(mInfos->popBack());
}

void TargetInfoList::append(const al::LiveActor* actor, const al::LiveActor* target, s32 timer) {
    if (mPool->isEmpty())
        return;

    TargetInfo* info = mPool->popBack();
    info->actor = actor;
    info->target = target;
    info->timer = timer;
    mInfos->pushBack(info);
}

void TargetInfoList::remove(const al::LiveActor* actor) {
    for (s32 i = 0; i < mInfos->size(); i++) {
        if ((*mInfos)[i]->actor == actor) {
            remove(i);
            return;
        }
    }
}

void TargetInfoList::remove(s32 index) {
    mPool->pushBack((*mInfos)[index]);
    mInfos->erase(index);
}

bool TargetInfoList::isInclude(const al::LiveActor* actor) const {
    for (s32 i = 0; i < mInfos->size(); i++)
        if ((*mInfos)[i]->actor == actor)
            return true;
    return false;
}

void TargetInfoList::elapse() {
    for (s32 i = 0; i < mInfos->size(); i++) {
        TargetInfo* info = (*mInfos)[i];
        if (info->timer != 0)
            info->timer--;
    }
}

void TargetInfoList::survive() {
    for (s32 i = 0; i < mInfos->size();) {
        if ((*mInfos)[i]->timer != 0) {
            i++;
        } else {
            mPool->pushBack((*mInfos)[i]);
            mInfos->erase(i);
        }
    }
}

void TargetInfoList::sort() {
    mInfos->sort();
}
