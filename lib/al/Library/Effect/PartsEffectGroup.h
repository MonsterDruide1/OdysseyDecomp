#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class EffectSystemInfo;
class IUseCamera;
class PartsEffect;

class PartsEffectGroup {
public:
    PartsEffectGroup();
    void init(s32, const EffectSystemInfo*, const char*, const sead::Vector3f*,
              const sead::Vector3f*, const sead::Matrix34f*, const IUseCamera*);
    PartsEffect* getEffect(s32) const;
    s32 getEffectNum() const;
    void update();
    void kill();

private:
    sead::PtrArray<PartsEffect> mEffects;
};

static_assert(sizeof(PartsEffectGroup) == 0x10);

}  // namespace al
