#pragma once

#include "Library/Shadow/ShadowMaskBase.h"

namespace al {

class ShadowMaskCastOvalCylinder : public ShadowMaskBase {
public:
    ShadowMaskCastOvalCylinder(const char*);

    ~ShadowMaskCastOvalCylinder() override;
    void declare(ShadowMaskDrawCategory) override;
    void update() override;
    void calcShadowMatrix(sead::Matrix34f*) override;
    void updateMulti() override;
    void addMulti() override;
    ShadowMaskType getShadowMaskType() const override;

    void calcOvalWrapMtxCylinder(sead::Matrix34f*, sead::Matrix34f, const sead::Vector3f&, f32);

    void init(const sead::Vector3f& scale, f32 dropLength, f32 expXZ, f32 expY, f32 distYBase,
              const sead::Vector3f& offset, const sead::Color4f& color) {
        mScale = scale;
        setDropLength(dropLength);
        mExpXZ = expXZ;
        mExpY = expY;
        mDistYBase = distYBase;
        setOffset(offset);
        setColor(color);
        setDrawCategory(ShadowMaskDrawCategory::ライトバッファ加算);
    }

private:
    sead::Vector3f mScale;
    void* _f8;
    s32 _100;
    f32 mExpXZ;
    f32 mExpY;
    f32 mDistYBase;
    void* _110[6];
};

static_assert(sizeof(ShadowMaskCastOvalCylinder) == 0x140);

}  // namespace al
