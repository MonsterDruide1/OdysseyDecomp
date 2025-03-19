#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace sead {
template <typename T>
class ObjArray;
}  // namespace sead

namespace al {
class ScreenPointCheckGroup;
class ScreenPointTarget;
class ScreenPointTargetHitInfo;
class ScreenPointer;

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
