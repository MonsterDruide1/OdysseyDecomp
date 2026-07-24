#pragma once

#include <container/seadPtrArray.h>
#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

class ShadowMaskBase;
class LiveActor;
struct ActorInitInfo;
class ByamlIter;
class ShadowMaskCastOvalCylinder;
class ShadowMaskCube;
class ShadowMaskCylinder;
class ShadowMaskSphere;

class ShadowMaskCtrl {
public:
    ShadowMaskCtrl(bool);
    void appendShadowMask(ShadowMaskBase*);
    ShadowMaskBase* findShadowMask(const char*) const;
    void hide();
    bool init(LiveActor*, const ActorInitInfo&, const ByamlIter&);
    bool init(LiveActor*, s32);
    void initAfterPlacement();
    void initShadowMaskNum(s32);
    void invalidate();
    bool isHide() const;
    void setupShadowMaskCastOvalCylinderParam(ShadowMaskCastOvalCylinder*) const;
    void setupShadowMaskCubeParam(ShadowMaskCube*) const;
    void setupShadowMaskCylinderParam(ShadowMaskCylinder*) const;
    void setupShadowMaskSphereParam(ShadowMaskSphere*) const;
    void show();
    void validate();

    s32 getShadowMaskNum() const { return mShadowMasks.size(); }

    ShadowMaskBase* getShadowMask(s32 index) const { return mShadowMasks[index]; }

    ShadowMaskBase* getShadowMask(u64 index) const { return mShadowMasks.data()[index]; }

private:
    struct ShadowMaskBaseInfo {
        void setPtr();
        void readIter(const ByamlIter&);

        const char* name;
        const char* shadowMaskType;
        const char* actorJointName;
        sead::Vector3f offset;
        sead::Color4f color;
        bool isApplyShadowIntensityUser;
        u8 shadowIntensityUser;
        bool isIgnoreHide;
        bool isFollowHostScale;
        const char* drawCategory;
        bool isShadowFixed;
        u8 _41[7];
        const char* setHeightEvenTargetName;
    };

    static_assert(sizeof(ShadowMaskBaseInfo) == 0x50);

    sead::PtrArray<ShadowMaskBase> mShadowMasks;
    sead::Matrix34f mMtx;
    void* mLiveActor;
    bool mIsIgnoreShadowMaskYaml;
};

}  // namespace al
