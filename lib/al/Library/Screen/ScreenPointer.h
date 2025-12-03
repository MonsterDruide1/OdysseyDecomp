#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class ScreenPointDirector;
class ScreenPointTarget;

struct ScreenPointTargetHitInfo {
    void* _0;
    ScreenPointTarget* target;
    f32 screenPointDistance;
    f32 directPointDistance;
    sead::Vector3f segmentPointStart;
    sead::Vector3f segmentPointEnd;
};

static_assert(sizeof(ScreenPointTargetHitInfo) == 0x30);

class ScreenPointer {
public:
    ScreenPointer(const ActorInitInfo& initInfo, const char* name);

    bool hitCheckSegment(const sead::Vector3f& posStart, const sead::Vector3f& posEnd);
    bool hitCheckScreenCircle(const sead::Vector2f& pos, f32 radius, f32 screenRadius);
    bool hitCheckLayoutCircle(const sead::Vector2f& pos, f32 radius, f32 layoutRadius,
                              s32 (*cmp)(const ScreenPointTargetHitInfo*,
                                         const ScreenPointTargetHitInfo*));
    bool recheckAndSortSegment(const sead::Vector3f& posStart, const sead::Vector3f& posEnd);
    ScreenPointTarget* getHitTarget(s32 index) const;
    bool isHitTarget(const ScreenPointTarget* target) const;

    s32 getHitTargetNum() const { return mHitTargetArray.size(); }

private:
    ScreenPointDirector* mDirector = nullptr;
    sead::ObjArray<ScreenPointTargetHitInfo> mHitTargetArray;
};

static_assert(sizeof(ScreenPointer) == 0x28);

}  // namespace al
