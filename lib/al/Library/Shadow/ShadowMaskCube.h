#pragma once

#include <gfx/seadColor.h>

#include "Library/Shadow/ShadowMaskBase.h"

namespace al {

class ShadowMaskCube : public ShadowMaskBase {
public:
    ShadowMaskCube(const char*);

    ~ShadowMaskCube() override;
    void declare(ShadowMaskDrawCategory) override;
    void update() override;
    void calcShadowMatrix(sead::Matrix34f*) override;
    void updateMulti() override;
    void addMulti() override;
    ShadowMaskType getShadowMaskType() const override;

    void tryInitTexture(const char*);

    // TODO: assign proper parameter names
    void init(f32 a1, f32 a2, f32 dropLength, const sead::Vector3f& a4, f32 a5,
              const sead::Vector3f& offset, const sead::Color4f& color) {
        _ec = a1;
        _f4 = a2;
        _f0 = 1.0f;
        setDropLength(dropLength);
        _f8.set(a4);
        _104 = a5;
        setOffset(offset);
        setColor(color);
        setDrawCategory(ShadowMaskDrawCategory::AO);
    }

private:
    f32 _ec;
    f32 _f0;
    f32 _f4;
    sead::Vector3f _f8;
    f32 _104;
    void* _108[46];
};

static_assert(sizeof(ShadowMaskCube) == 0x278);

}  // namespace al
