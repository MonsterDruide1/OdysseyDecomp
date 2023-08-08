#pragma once

#include <container/seadPtrArray.h>

namespace al {

class ShadowMaskBase;
class LiveActor;
class ActorInitInfo;
class ByamlIter;
class ShadowMaskCastOvalCylinder;
class ShadowMaskCube;
class ShadowMaskCylinder;
class ShadowMaskSphere;

class ShadowMaskCtrl {
public:
    ShadowMaskCtrl(bool);
    void appendShadowMask(ShadowMaskBase*);
    ShadowMaskBase* findShadowMask(const char*);
    void hide();
    bool init(LiveActor*, const ActorInitInfo&, const ByamlIter&);
    bool init(LiveActor*, int);
    void initAfterPlacement();
    void initShadowMaskNum(int);
    void invalidate();
    bool isHide();
    void setupShadowMaskCastOvalCylinderParam(ShadowMaskCastOvalCylinder*);
    void setupShadowMaskCubeParam(ShadowMaskCube*);
    void setupShadowMaskCylinderParam(ShadowMaskCylinder*);
    void setupShadowMaskSphereParam(ShadowMaskSphere*);
    void show();
    void validate();

private:
    sead::PtrArray<ShadowMaskBase> array;
    void* gap[6];
    void* mLiveActor;
    bool mIsIgnoreShadowMaskYaml;
};

}  // namespace al