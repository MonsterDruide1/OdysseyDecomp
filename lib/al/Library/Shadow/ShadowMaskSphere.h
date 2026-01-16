#pragma once

#include "Library/Shadow/ShadowMaskBase.h"

namespace al {

class ShadowMaskSphere : public ShadowMaskBase {
public:
    ShadowMaskSphere(const char*);

    ~ShadowMaskSphere() override;
    void declare(ShadowMaskDrawCategory) override;
    void update() override;
    void calcShadowMatrix(sead::Matrix34f*) override;
    void updateMulti() override;
    void addMulti() override;
    ShadowMaskType getShadowMaskType() const override;

private:
    void* _f0[2];
};

static_assert(sizeof(ShadowMaskSphere) == 0x100);

}  // namespace al
