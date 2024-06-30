#pragma once

#include <container/seadObjArray.h>
#include <math/seadVector.h>

namespace al {
class ScreenPointer;
class ScreenPointCheckGroup;
class ScreenPointTarget;
class ScreenPointTargetHitInfo;

class ScreenPointDirector {
public:
    ScreenPointDirector();
    void registerTarget(ScreenPointTarget*);
    void setCheckGroup(ScreenPointTarget*);
    bool hitCheckSegment(ScreenPointer*, sead::ObjArray<ScreenPointTargetHitInfo>*, s32,
                         const sead::Vector3f&, const sead::Vector3f&);
    bool hitCheckScreenCircle(ScreenPointer*, sead::ObjArray<ScreenPointTargetHitInfo>*, s32,
                              const sead::Vector2f&, f32, f32);
    bool hitCheckLayoutCircle(ScreenPointer*, sead::ObjArray<ScreenPointTargetHitInfo>*, s32,
                              const sead::Vector2f&, f32, f32,
                              s32 (*)(const ScreenPointTargetHitInfo*,
                                      const ScreenPointTargetHitInfo*));

private:
    ScreenPointCheckGroup* mCheckGroup;
};

static_assert(sizeof(ScreenPointDirector) == 0x8);

}  // namespace al
