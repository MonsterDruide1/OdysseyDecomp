#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class TargetInfo {
public:
    TargetInfo();
    f32 dist() const;

    bool operator<(const TargetInfo& other) const { return dist() < other.dist(); }

    const al::LiveActor* mActor = nullptr;
    s32 mTimer = 0;
    const al::LiveActor* mTarget = nullptr;
};

static_assert(sizeof(TargetInfo) == 0x18);

class TargetInfoList {
public:
    TargetInfoList();
    void clear();
    void append(const al::LiveActor* actor, const al::LiveActor* target, s32 timer);
    void remove(const al::LiveActor* actor);
    void remove(s32 index);
    bool isInclude(const al::LiveActor* actor) const;
    void elapse();
    void survive();
    void sort();

private:
    sead::PtrArray<TargetInfo>* mInfos;
    sead::PtrArray<TargetInfo>* mPool;
};
