#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <prim/seadEnum.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class ByamlIter;
class MtxConnector;
enum class ShadowMaskType;

SEAD_ENUM(ShadowMaskDrawCategory, シャドウ, AO, ライトスケール, ライトバッファ加算)

class ShadowMaskBase {
public:
    ShadowMaskBase(const char*);

    virtual ~ShadowMaskBase();
    virtual void declare(ShadowMaskDrawCategory);
    virtual void update();
    virtual void initAfterPlacement();
    virtual void calcShadowMatrix(sead::Matrix34f*);
    virtual void createMtxConnector();
    virtual void readParam(const ByamlIter&);
    virtual void updateMulti();
    virtual void addMulti();
    virtual ShadowMaskType getShadowMaskType() const = 0;

    void setHost(const LiveActor*);
    void setDrawCategory(const char*);

    MtxConnector* getMtxConnector() const { return mMtxConnector; }

    ShadowMaskDrawCategory getDrawCategory() const { return mDrawCategory; }

    void setDropLength(f32 dropLength) { mDropLength = dropLength; }

    void setOffset(const sead::Vector3f& offset) { mOffset.set(offset); }

    void setColor(const sead::Color4f& color) { mColor = color; }

    void setDrawCategory(ShadowMaskDrawCategory drawCategory) { mDrawCategory = drawCategory; }

private:
    const LiveActor* mHost;
    MtxConnector* mMtxConnector;
    sead::Vector3f mOffset;
    sead::Color4f mColor;
    sead::Vector3f _34;
    f32 mDropLength;
    s32 pad2;
    void* _48[1];
    const char* mName;
    bool mIsFixedIntensity;
    bool mIsFollowHostScale;
    bool mIsIgnoreHide;
    bool pad4;
    ShadowMaskDrawCategory mDrawCategory;
    bool mIsShadowFixed;
    bool pad3[7];
    const char* mActorJointName;
    void* _70[7];
    sead::FixedSafeString<32> mSetHeightEvenTargetName;
    s32 _e0[3];
};

static_assert(sizeof(ShadowMaskBase) == 0xf0);

}  // namespace al
