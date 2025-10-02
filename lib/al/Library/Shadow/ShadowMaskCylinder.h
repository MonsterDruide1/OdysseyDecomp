#pragma once

#include "Library/Shadow/ShadowMaskBase.h"

namespace al {

class ShadowMaskCylinder : public ShadowMaskBase {
public:
    ShadowMaskCylinder(const char*);

    ~ShadowMaskCylinder() override;
    void declare(ShadowMaskDrawCategory) override;
    void update() override;
    void calcShadowMatrix(sead::Matrix34f*) override;
    void updateMulti() override;
    void addMulti() override;
    ShadowMaskType getShadowMaskType() const override;

    // TODO: assign proper parameter names
    void init(f32 a1, f32 dropLength, f32 a3, f32 a4, f32 a5, const sead::Vector3f& offset,
              const sead::Color4f& color) {
        _ec = a1;
        _f4 = a1;
        _f0 = 1.0f;
        setDropLength(dropLength);
        _104 = a3;
        _108 = a4;
        _10c = a5;
        setOffset(offset);
        setColor(color);
        setDrawCategory(ShadowMaskDrawCategory::AO);
    }

private:
    f32 _ec;
    f32 _f0;
    f32 _f4;
    void* _f8;
    s32 _100;
    f32 _104;
    f32 _108;
    f32 _10c;
    bool _110;
};

static_assert(sizeof(ShadowMaskCylinder) == 0x118);

}  // namespace al
