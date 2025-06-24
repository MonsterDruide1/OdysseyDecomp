#pragma once

namespace al {
class ShadowMaskBase;

class ShadowMaskDirector {
public:
    void registerShadowMask(al::ShadowMaskBase*);

private:
    void* _0[0x8D8/8];
};

static_assert(sizeof(ShadowMaskDirector) == 0x8D8);

}
