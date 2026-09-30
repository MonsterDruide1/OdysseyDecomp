#pragma once

#include <basis/seadTypes.h>

namespace sead::ptcl {
class Config;
}  // namespace sead::ptcl

namespace al {
class EffectSystem;

class PtclSystem {
public:
    PtclSystem(const sead::ptcl::Config&, EffectSystem*);

    s32 getNumResource() const;
    void entryResourceEnd();
    void* getEffectEnvParam();

    const EffectSystem& getEffectSystem() const { return *mEffectSystem; }

private:
    u8 _0[0x2900];
    EffectSystem* mEffectSystem = nullptr;
    void* mEffectEnvParam = nullptr;
};
}  // namespace al
