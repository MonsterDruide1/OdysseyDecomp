#include "MapObj/ElectricTargetInfo.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorPoseUtil.h"

TargetInfo::TargetInfo() {}

f32 TargetInfo::dist() const {
    sead::Vector3f targetPos = al::getTrans(mTarget);
    const sead::Vector3f& actorPos = al::getTrans(mActor);
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
    if (mPool->size() != 0) {
        TargetInfo* info = mPool->popBack();
        info->mActor = actor;
        info->mTarget = target;
        info->mTimer = timer;
        mInfos->pushBack(info);
    }
}

void TargetInfoList::remove(const al::LiveActor* actor) {
    for (s32 i = 0; i < mInfos->size(); i++) {
        if (mInfos->at(i)->mActor == actor) {
            mPool->pushBack(mInfos->at(i));
            mInfos->erase(i);
            return;
        }
    }
}

void TargetInfoList::remove(s32 index) {
    mPool->pushBack(mInfos->at(index));
    mInfos->erase(index);
}

bool TargetInfoList::isInclude(const al::LiveActor* actor) const {
    for (s32 i = 0; i < mInfos->size(); i++)
        if (mInfos->at(i)->mActor == actor)
            return true;
    return false;
}

void TargetInfoList::elapse() {
    for (s32 i = 0; i < mInfos->size(); i++) {
        TargetInfo* info = mInfos->at(i);
        if (info->mTimer != 0)
            info->mTimer--;
    }
}

void TargetInfoList::survive() {
    for (s32 i = 0; i < mInfos->size();) {
        if (mInfos->at(i)->mTimer != 0) {
            i++;
        } else {
            mPool->pushBack(mInfos->at(i));
            mInfos->erase(i);
        }
    }
}

void TargetInfoList::sort() {
    mInfos->sort();
}
