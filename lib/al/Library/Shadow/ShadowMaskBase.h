#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <prim/seadEnum.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class ByamlIter;
class MtxConnector;
enum class ShadowMaskType : s32 { None = 0, Sphere = 1, Cylinder = 2, Cube = 3 };

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

    const LiveActor* getHost() const { return mHost; }

    MtxConnector* getMtxConnector() const { return mMtxConnector; }

    const char* getName() const { return mName; }

    ShadowMaskDrawCategory getDrawCategory() const { return mDrawCategory; }

    void setShadowFixed(bool isFixed) { mIsShadowFixed = isFixed; }

    bool isIgnoreHide() const { return mIsIgnoreHide; }

    bool isValid() const { return mIsValid; }

    void setValid(bool isValid) { mIsValid = isValid; }

    bool isHidden() const { return mIsHidden; }

    void setHidden(bool isHidden) { mIsHidden = isHidden; }

    const sead::Vector3f& getDropDir() const { return mDropDir; }

    void setDropDir(const sead::Vector3f& dropDir) {
        mDropDir.x = dropDir.x;
        mDropDir.y = dropDir.y;
        mDropDir.z = dropDir.z;
    }

    void setDropDir(f32 x, f32 y, f32 z) {
        mDropDir.x = x;
        mDropDir.y = y;
        mDropDir.z = z;
    }

    f32 getDropLength() const { return mDropLength; }

    void setDropLength(f32 dropLength) { mDropLength = dropLength; }

    void scaleDropLength(f32 scale) { mDropLength *= scale; }

    void setDropLengthPlaneNormal(const sead::Vector3f& planeNormal) {
        mDropLengthPlaneNormal.x = planeNormal.x;
        mDropLengthPlaneNormal.y = planeNormal.y;
        mDropLengthPlaneNormal.z = planeNormal.z;
    }

    f32 getSizeX() const { return mSizeX; }

    void setSizeX(f32 sizeX) { mSizeX = sizeX; }

    sead::Vector3f getDropLengthEvenSize() const {
        return {mDropLengthEvenSizeX, mDropLengthEvenSizeY, mDropLengthEvenSizeZ};
    }

    sead::Vector3f getDropLengthEvenCenter() const {
        return {mDropLengthEvenCenterX, mDropLengthEvenCenterY, mDropLengthEvenCenterZ};
    }

    f32 getDropLengthEvenSizeX() const { return mDropLengthEvenSizeX; }

    f32 getDropLengthEvenSizeY() const { return mDropLengthEvenSizeY; }

    f32 getDropLengthEvenSizeZ() const { return mDropLengthEvenSizeZ; }

    f32 getDropLengthEvenCenterX() const { return mDropLengthEvenCenterX; }

    f32 getDropLengthEvenCenterY() const { return mDropLengthEvenCenterY; }

    f32 getDropLengthEvenCenterZ() const { return mDropLengthEvenCenterZ; }

    const sead::Vector3f& getOffset() const { return mOffset; }

    void setOffset(const sead::Vector3f& offset) { mOffset.set(offset); }

    const sead::Color4f& getColor() const { return mColor; }

    void setColor(const sead::Color4f& color) { mColor = color; }

    f32 getIntensity() const { return mColor.r; }

    void setIntensity(f32 intensity) { mColor.r = intensity; }

    void setDrawCategory(ShadowMaskDrawCategory drawCategory) { mDrawCategory = drawCategory; }

private:
    const LiveActor* mHost;
    MtxConnector* mMtxConnector;
    sead::Vector3f mOffset;
    sead::Color4f mColor;
    sead::Vector3f mDropDir;
    f32 mDropLength;
    sead::Vector3f mDropLengthPlaneNormal;
    const char* mName;
    bool mIsFixedIntensity;
    bool mIsFollowHostScale;
    bool mIsIgnoreHide;
    bool pad4;
    ShadowMaskDrawCategory mDrawCategory;
    bool mIsShadowFixed;
    bool pad3[7];
    const char* mActorJointName;
    u8 _70[2];
    bool mIsValid;
    bool mIsHidden;
    u8 _74[4];
    f32 mDropLengthEvenSizeX;
    u8 _7c[4];
    f32 mDropLengthEvenCenterX;
    u8 _84[4];
    f32 mDropLengthEvenSizeY;
    u8 _8c[4];
    f32 mDropLengthEvenCenterY;
    u8 _94[4];
    f32 mDropLengthEvenSizeZ;
    u8 _9c[4];
    f32 mDropLengthEvenCenterZ;
    u8 _a4[4];
    sead::FixedSafeString<32> mSetHeightEvenTargetName;
    s32 _e0[3];
    f32 mSizeX;
};

static_assert(sizeof(ShadowMaskBase) == 0xf0);

}  // namespace al
