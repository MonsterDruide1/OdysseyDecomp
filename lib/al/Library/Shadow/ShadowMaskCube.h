#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

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

    f32 getSizeZ() const { return mSizeZ; }

    void setSizeZ(f32 sizeZ) { mSizeZ = sizeZ; }

    f32 getTextureFixedScale() const { return mTextureFixedScale; }

    void setTextureFixedScale(f32 scale) { mTextureFixedScale = scale; }

    void setTextureOffset(f32 x, f32 y) { mTextureOffset.set(x, y); }

    void setTextureScroll(bool isScroll) { mIsTextureScroll = isScroll; }

    // TODO: assign proper parameter names
    void init(f32 a1, f32 a2, f32 dropLength, const sead::Vector3f& a4, f32 a5,
              const sead::Vector3f& offset, const sead::Color4f& color) {
        setSizeX(a1);
        mSizeZ = a2;
        mSizeY = 1.0f;
        setDropLength(dropLength);
        _f8.set(a4);
        _104 = a5;
        setOffset(offset);
        setColor(color);
        setDrawCategory(ShadowMaskDrawCategory::AO);
    }

private:
    f32 mSizeY;
    f32 mSizeZ;
    sead::Vector3f _f8;
    f32 _104;
    void* _108[38];
    void* _238;
    void* _240;
    sead::Vector2f mTextureOffset;
    sead::Vector2f _250;
    sead::Vector2f _258;
    sead::Vector2f _260;
    sead::Vector2f _268;
    f32 mTextureFixedScale;
    bool mIsTextureScroll;
    u8 _275[3];
};

static_assert(sizeof(ShadowMaskCube) == 0x278);

}  // namespace al
